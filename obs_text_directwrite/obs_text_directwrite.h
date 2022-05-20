#pragma once
#include <util/platform.h>
#include <util/util.hpp>
#include <obs-module.h>
#include <sys/stat.h>
#include <windows.h>
#include <algorithm>
#include <string>
#include <memory>
#include <locale>
#include <vector>
#include <math.h>
#include <codecvt>
#include <optional>
#include <d3d11.h>
#include <dwrite.h>
#include <d2d1.h>
#include <d3d11.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wrl.h>

#include "obs_text_renderer.h"
#include "gumbo/gumbo.h"

#ifndef clamp
#define clamp(val, min_val, max_val) \
	if (val < min_val)           \
		val = min_val;       \
	else if (val > max_val)      \
		val = max_val;
#endif

#define MIN_SIZE_CX 2.0
#define MIN_SIZE_CY 2.0
#define MAX_SIZE_CX 16384.0
#define MAX_SIZE_CY 16384.0

using namespace Microsoft::WRL;

enum class gradient_mode : uint32_t { none = 0, two_colour = 2, three_colour = 3, four_colour = 4 };

enum class format_flags {
	none,
	bold,
	italic,
	underline,
	strikethrough,
};

struct dwrite_run {
	int start;
	int length;
	format_flags format;
	std::optional<float> size;
	std::optional<uint32_t> color;
};

struct obs_dwrite_text_source {

	obs_source_t *source = nullptr;
	gs_texture_t *tex = nullptr;

	uint32_t cx = 0;
	uint32_t cy = 0;

	ComPtr<IDWriteFactory4> pDWriteFactory = nullptr;
	ComPtr<IDWriteTextFormat> pTextFormat = nullptr;
	ComPtr<IDWriteTextLayout> pTextLayout = nullptr;

	ComPtr<ID2D1Factory4> pD2DFactory = nullptr;
	ComPtr<ID2D1Device4> pD2DDevice = nullptr;
	ComPtr<ID2D1DeviceContext4> pD2DDeviceContext = nullptr;
	ComPtr<ID2D1Brush> pFillBrush = nullptr;
	ComPtr<ID2D1Brush> pOutlineBrush = nullptr;
	ComPtr<ID2D1Bitmap1> pTarget = nullptr;

	ComPtr<obs_text_renderer> pTextRenderer = nullptr;

	bool read_from_file = false;
	std::string file;
	time_t file_timestamp = 0;
	float update_time_elapsed = 0.0f;

	std::wstring text;
	std::wstring face;
	int face_size = 0;
	uint32_t color = 0xFFFFFF;
	uint32_t color2 = 0xFFFFFF;
	uint32_t color3 = 0xFFFFFF;
	uint32_t color4 = 0xFFFFFF;

	gradient_mode gradient_count = gradient_mode::none;

	float gradient_dir = 0;
	float gradient_x = 0;
	float gradient_y = 0;
	float gradient2_x = 0;
	float gradient2_y = 0;

	uint32_t opacity = 100;
	uint32_t opacity2 = 100;
	uint32_t bk_color = 0;
	uint32_t bk_opacity = 0;

	DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_REGULAR;
	DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL;
	DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL;
	DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_LEADING;
	DWRITE_PARAGRAPH_ALIGNMENT valign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
	DWRITE_WORD_WRAPPING wrap = DWRITE_WORD_WRAPPING_WRAP;

	// bool bold = false;
	bool italic = false;
	bool underline = false;
	bool strikeout = false;
	bool color_fonts = true;
	bool antialias = true;
	//bool vertical = false;

	bool use_outline = false;
	float outline_size = 0.0f;
	uint32_t outline_color = 0;
	uint32_t outline_opacity = 100;

	bool use_extents = false;
	int32_t extents_cx = 0;
	int32_t extents_cy = 0;

	int text_transform = 0;
	int text_trimming = 0;

	bool chatlog_mode = false;
	int chatlog_lines = 6;

	bool use_html = false;
	std::vector<dwrite_run> runs;

	/* --------------------------- */

	inline obs_dwrite_text_source(obs_source_t *source_, obs_data_t *settings) : source(source_)
	{
		obs_enter_graphics();
		auto hr = InitializeDirectWrite();
		obs_leave_graphics();

		if (FAILED(hr))
			throw std::exception("Failed to init DirectWrite");

		obs_source_update(source, settings);
	}

	inline ~obs_dwrite_text_source()
	{
		if (tex) {
			obs_enter_graphics();
			gs_texture_destroy(tex);
			obs_leave_graphics();
		}

		ReleaseResource();
	}

	void UpdateFont();
	void CalculateGradientAxis(float width, float height);
	HRESULT InitializeDirectWrite();
	void ReleaseResource();
	void UpdateBrush(ComPtr<ID2D1DeviceContext4> pRT, ID2D1Brush **ppOutlineBrush, ID2D1Brush **ppFillBrush,
			 float width, float height);
	void RenderText();
	void LoadFileText();

	const char *GetMainString(const char *str);
	void TransformText();
	std::string ProcessHtml(GumboNode *node, int position);

	inline void Update(obs_data_t *settings);
	inline void Tick(float seconds);
	inline void Render(gs_effect_t *effect);
};
