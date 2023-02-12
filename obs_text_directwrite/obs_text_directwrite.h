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
#include <d2d1effects.h>
#include <wrl.h>

#include <winrt/base.h>

#include "obs_text_renderer.h"

#define do_log(level, format, ...) \
	blog(level, "[obs-text-dwrite: '%s'] " format, obs_source_get_name(this->source), ##__VA_ARGS__)

#define error(format, ...) do_log(LOG_ERROR, "ERROR: " format, ##__VA_ARGS__)
#define warn(format, ...) do_log(LOG_WARNING, "WARN: " format, ##__VA_ARGS__)
#define info(format, ...) do_log(LOG_INFO, "INFO: " format, ##__VA_ARGS__)
#define debug(format, ...) do_log(LOG_DEBUG, "DEBUG: " format, ##__VA_ARGS__)

#ifndef clamp_assign
#define clamp_assign(val, min_val, max_val) \
	if (val < min_val)                  \
		val = min_val;              \
	else if (val > max_val)             \
		val = max_val;
#endif

#define MIN_SIZE_CX 2.0
#define MIN_SIZE_CY 2.0
#define MAX_SIZE_CX 16384.0
#define MAX_SIZE_CY 16384.0

#define MAX_GRADIENT_STOPS 16

using namespace Microsoft::WRL;

enum class format_flags {
	none,
	bold,
	italic,
	underline,
	strikethrough,
};

struct dwrite_run_t {
	uint32_t start;
	uint32_t length;
	format_flags format;
	std::optional<float> size;
	std::optional<uint32_t> color;
};

struct gradient_stop_t {
	uint32_t color;
	float offset;
};

struct gradient_axis_t {
	float x1, y1, x2, y2;
};

struct thickness_t {
	float top, left, bottom, right;

	inline bool operator==(const thickness_t &other)
	{
		return left == other.left && right == other.right && top == other.top && bottom == other.bottom;
	}
	inline bool operator!=(const thickness_t &other) { return !(*this == other); }
};

struct usize_t {
	uint32_t width, height;

	inline bool operator==(const usize_t &other) { return width == other.width && height == other.height; }
	inline bool operator!=(const usize_t &other) { return !(*this == other); }
};

struct obs_dwrite_text_source {

	obs_source_t *source = nullptr;

	gs_texture_t *textTexture = nullptr;
	gs_texture_t *targetTexture = nullptr;

	usize_t size = {0, 0};
	thickness_t padding = {0, 0, 0, 0};

	winrt::com_ptr<IDWriteFactory2> pDWriteFactory = nullptr;
	winrt::com_ptr<IDWriteTextFormat> pTextFormat = nullptr;
	winrt::com_ptr<IDWriteTextLayout> pTextLayout = nullptr;

	winrt::com_ptr<ID2D1Factory1> pD2DFactory = nullptr;
	winrt::com_ptr<ID2D1Device1> pD2DDevice = nullptr;
	winrt::com_ptr<ID2D1DeviceContext1> pD2DContext = nullptr;
	winrt::com_ptr<ID2D1Brush> pFillBrush = nullptr;
	winrt::com_ptr<ID2D1Brush> pOutlineBrush = nullptr;

	winrt::com_ptr<ID2D1Bitmap1> pTextTarget = nullptr;
	winrt::com_ptr<ID2D1Bitmap1> pTarget = nullptr;

	winrt::com_ptr<ID2D1Effect> pEffect = nullptr;

	winrt::com_ptr<OBSTextRenderer> pTextRenderer = nullptr;

	bool read_from_file = false;
	std::string file;
	time_t file_timestamp = 0;
	float update_time_elapsed = 0.0f;

	std::wstring text;
	std::wstring face;
	int face_size = 0;

	int32_t gradient_count = 1;
	gradient_stop_t gradient_stops[MAX_GRADIENT_STOPS];

	float gradient_angle = 0;

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

	bool use_shadow = false;
	float shadow_radius = 0.0f;
	float shadow_offset_x = 0.0f;
	float shadow_offset_y = 0.0f;
	uint32_t shadow_color = 0;
	uint32_t shadow_opacity = 100;

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
	std::vector<dwrite_run_t> runs;

	/* --------------------------- */

	inline obs_dwrite_text_source(obs_source_t *source_, obs_data_t *settings) : source(source_)
	{
		obs_enter_graphics();

		try {
			init_dwrite();
		} catch (const winrt::hresult_error &e) {
			error("DirectWrite initialization failed with code %#08x (%s)", e.code().value,
			      to_string(e.message()).c_str());
		}

		obs_leave_graphics();

		obs_source_update(source, settings);
	}

	inline ~obs_dwrite_text_source()
	{
		if (targetTexture) {
			obs_enter_graphics();
			gs_texture_destroy(targetTexture);
			obs_leave_graphics();
		}

		if (textTexture) {
			obs_enter_graphics();
			gs_texture_destroy(textTexture);
			obs_leave_graphics();
		}

		release();
	}

	inline uint32_t get_width()
	{
		return use_extents && extents_cx != -1
			       ? extents_cx
			       : (uint32_t)std::clamp(size.width + padding.left + padding.right,
						      (float)MIN_SIZE_CX, (float)MAX_SIZE_CX);
	}

	inline uint32_t get_height()
	{
		return use_extents && extents_cy != -1
			       ? extents_cy
			       : (uint32_t)std::clamp(size.height + padding.top + padding.bottom,
						      (float)MIN_SIZE_CY, (float)MAX_SIZE_CY);
	}

	gradient_axis_t calculate_gradient_axis(float width, float height);

	void init_dwrite();
	void release();

	void update_font();
	void update_brush(float width, float height);
	void update_effects();
	void draw_text();

	bool create_render_target_d3d11(usize_t &size, thickness_t &padding);

	void load_file();

	const char *get_string(const char *str);
	void transform_text();

	inline void Update(obs_data_t *settings);
	inline void Tick(float seconds);
	inline void Render(gs_effect_t *effect);
};
