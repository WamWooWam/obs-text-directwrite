
#include "obs_text_directwrite.h"

/* ------------------------------------------------------------------------- */

#define S_FONT "font"
#define S_FONT_GROUP "font_group"
#define S_USE_FILE "read_from_file"
#define S_FILE "file"
#define S_TEXT "text"
#define S_TEXT_GROUP "text_group"
#define S_COLOR "color"
#define S_GRADIENT "gradient"
#define S_GRADIENT_DIR "gradient_dir"
#define S_GRADIENT_OPACITY "gradient_opacity"

#define S_GRADIENT_EX "colors"
#define S_GRADIENT_EX_COUNT "gradient_count"
#define S_GRADIENT_EX_COLOR "gradient_color_%d"
#define S_GRADIENT_EX_OFFSET "gradient_offset_%d"
#define S_GRADIENT_EX_DIR "gradient_dir"
#define S_GRADIENT_EX_OPACITY "gradient_opacity"

#define S_ALIGN "align"
#define S_VALIGN "valign"
#define S_OPACITY "opacity"

#define S_BKGROUP "bk_group"
#define S_BKOPACITY "bk_opacity"
#define S_BKCOLOR "bk_color"
#define S_BKOPACITY "bk_opacity"

//#define S_VERTICAL                      "vertical"
#define S_LAYOUT "layout"

#define S_SHADOW "shadow"
#define S_SHADOW_GROUP "shadow_group"
#define S_SHADOW_OFFSET_X "shadow_offset_x"
#define S_SHADOW_OFFSET_Y "shadow_offset_y"
#define S_SHADOW_RADIUS "shadow_radius"
#define S_SHADOW_COLOR "shadow_color"
#define S_SHADOW_OPACITY "shadow_opacity"

#define S_OUTLINE "outline"
#define S_OUTLINE_GROUP "outline_group"
#define S_OUTLINE_SIZE "outline_size"
#define S_OUTLINE_COLOR "outline_color"
#define S_OUTLINE_OPACITY "outline_opacity"

#define S_CHATLOG_MODE "chatlog"
#define S_CHATLOG_LINES "chatlog_lines"

#define S_EXTENTS "extents"
#define S_EXTENTS_CX "extents_cx"
#define S_EXTENTS_CY "extents_cy"

#define S_ALIGN_LEFT "left"
#define S_ALIGN_CENTER "center"
#define S_ALIGN_RIGHT "right"
#define S_ALIGN_JUSTIFIED "justified"

#define S_VALIGN_TOP "top"
#define S_VALIGN_CENTER S_ALIGN_CENTER
#define S_VALIGN_BOTTOM "bottom"

#define S_WRAP_MODE "wrap_mode"
#define S_WRAP_MODE_NONE "none"
#define S_WRAP_MODE_WRAP "wrap"
#define S_WRAP_MODE_WRAP_WHOLE_WORDS "wrap_whole_words"
#define S_WRAP_MODE_WRAP_CHARACTER "wrap_character"

#define S_COLOR_FONTS "color_fonts"

#define S_ANTIALIASING "antialiasing"

#define S_TRANSFORM "transform"
#define S_TRANSFORM_NONE 0
#define S_TRANSFORM_UPPERCASE 1
#define S_TRANSFORM_LOWERCASE 2
#define S_TRANSFORM_STARTCASE 3

#define S_TRIMMING "trimming"
#define S_TRIMMING_NONE 0
#define S_TRIMMING_CHARACTER_ELLIPSIS 0
#define S_TRIMMING_WORD_ELLIPSIS 2

#define S_FONT_WEIGHT "font_weight"
#define S_FONT_WEIGHT_AUTO -1
#define S_FONT_WEIGHT_100 DWRITE_FONT_WEIGHT_THIN
#define S_FONT_WEIGHT_200 DWRITE_FONT_WEIGHT_EXTRA_LIGHT
#define S_FONT_WEIGHT_300 DWRITE_FONT_WEIGHT_LIGHT
#define S_FONT_WEIGHT_350 DWRITE_FONT_WEIGHT_SEMI_LIGHT
#define S_FONT_WEIGHT_400 DWRITE_FONT_WEIGHT_NORMAL
#define S_FONT_WEIGHT_500 DWRITE_FONT_WEIGHT_MEDIUM
#define S_FONT_WEIGHT_600 DWRITE_FONT_WEIGHT_SEMI_BOLD
#define S_FONT_WEIGHT_700 DWRITE_FONT_WEIGHT_BOLD
#define S_FONT_WEIGHT_800 DWRITE_FONT_WEIGHT_EXTRA_BOLD
#define S_FONT_WEIGHT_900 DWRITE_FONT_WEIGHT_BLACK
#define S_FONT_WEIGHT_950 DWRITE_FONT_WEIGHT_EXTRA_BLACK

#define S_FONT_STRETCH "font_stretch"
#define S_FONT_STRETCH_AUTO -1
#define S_FONT_STRETCH_ULTRA_CONDENSED DWRITE_FONT_STRETCH_ULTRA_CONDENSED
#define S_FONT_STRETCH_EXTRA_CONDENSED DWRITE_FONT_STRETCH_EXTRA_CONDENSED
#define S_FONT_STRETCH_CONDENSED DWRITE_FONT_STRETCH_CONDENSED
#define S_FONT_STRETCH_SEMI_CONDENSED DWRITE_FONT_STRETCH_SEMI_CONDENSED
#define S_FONT_STRETCH_NORMAL DWRITE_FONT_STRETCH_NORMAL
#define S_FONT_STRETCH_SEMI_EXPANDED DWRITE_FONT_STRETCH_SEMI_EXPANDED
#define S_FONT_STRETCH_EXPANDED DWRITE_FONT_STRETCH_EXPANDED
#define S_FONT_STRETCH_EXTRA_EXPANDED DWRITE_FONT_STRETCH_EXTRA_EXPANDED
#define S_FONT_STRETCH_ULTRA_EXPANDED DWRITE_FONT_STRETCH_ULTRA_EXPANDED

#define S_FONT_FACE "face"
#define S_FONT_SIZE "size"
#define S_FONT_FLAGS "flags"

#define S_ADVANCED "advanced"

#define T_(v) obs_module_text(v)
#define T_FONT T_("Font")
#define T_USE_FILE T_("ReadFromFile")
#define T_FILE T_("TextFile")
#define T_TEXT T_("Text")
#define T_COLOR T_("Color")
#define T_COLORS T_("Colors")
#define T_OFFSET T_("Offset")
#define T_GRADIENT T_("Gradient")
#define T_GRADIENT_DIR T_("Gradient.Direction")
#define T_GRADIENT_OPACITY T_("Gradient.Opacity")
#define T_ALIGN T_("Alignment")
#define T_VALIGN T_("VerticalAlignment")
#define T_OPACITY T_("Opacity")

#define T_BKGROUP T_("Background")
#define T_BKCOLOR T_("BkColor")
#define T_BKOPACITY T_("BkOpacity")

#define T_LAYOUT T_("Layout")

#define T_SHADOW T_("Shadow")
#define T_SHADOW_OFFSET_X T_("Shadow.OffsetX")
#define T_SHADOW_OFFSET_Y T_("Shadow.OffsetY")
#define T_SHADOW_RADIUS T_("Shadow.Radius")
#define T_SHADOW_COLOR T_("Shadow.Color")
#define T_SHADOW_OPACITY T_("Shadow.Opacity")

#define T_OUTLINE T_("Outline")
#define T_OUTLINE_SIZE T_("Outline.Size")
#define T_OUTLINE_COLOR T_("Outline.Color")
#define T_OUTLINE_OPACITY T_("Outline.Opacity")

#define T_CHATLOG_MODE T_("ChatlogMode")
#define T_CHATLOG_LINES T_("ChatlogMode.Lines")

#define T_EXTENTS T_("UseCustomExtents")
#define T_EXTENTS_CX T_("Width")
#define T_EXTENTS_CY T_("Height")

#define T_FILTER_TEXT_FILES T_("Filter.TextFiles")
#define T_FILTER_ALL_FILES T_("Filter.AllFiles")

#define T_ALIGN_LEFT T_("Alignment.Left")
#define T_ALIGN_CENTER T_("Alignment.Center")
#define T_ALIGN_RIGHT T_("Alignment.Right")
#define T_ALIGN_JUSTIFIED T_("Alignment.Justified")

#define T_VALIGN_TOP T_("VerticalAlignment.Top")
#define T_VALIGN_CENTER T_ALIGN_CENTER
#define T_VALIGN_BOTTOM T_("VerticalAlignment.Bottom")

#define T_WRAP_MODE T_("WordWrap")
#define T_WRAP_MODE_NONE T_("WordWrap.None")
#define T_WRAP_MODE_WRAP T_("WordWrap.Wrap")
#define T_WRAP_MODE_WRAP_WHOLE_WORDS T_("WordWrap.WrapWholeWords")
#define T_WRAP_MODE_WRAP_CHARACTER T_("WordWrap.WrapCharacter")

#define T_COLOR_FONTS T_("ColorFonts")
#define T_ANTIALIASING T_("Antialiasing")

#define T_TRANSFORM T_("Transform")
#define T_TRANSFORM_NONE T_("Transform.None")
#define T_TRANSFORM_UPPERCASE T_("Transform.Uppercase")
#define T_TRANSFORM_LOWERCASE T_("Transform.Lowercase")
#define T_TRANSFORM_STARTCASE T_("Transform.Startcase")

#define T_TRIMMING T_("TextTrimming")
#define T_TRIMMING_NONE T_("TextTrimming.None")
#define T_TRIMMING_CHARACTER_ELLIPSIS T_("TextTrimming.CharacterEllipsis")
#define T_TRIMMING_WORD_ELLIPSIS T_("TextTrimming.WordEllipsis")

#define T_FONT_WEIGHT T_("FontWeight")
#define T_FONT_WEIGHT_AUTO T_("FontWeight.Auto")
#define T_FONT_WEIGHT_100 T_("FontWeight.100")
#define T_FONT_WEIGHT_200 T_("FontWeight.200")
#define T_FONT_WEIGHT_300 T_("FontWeight.300")
#define T_FONT_WEIGHT_350 T_("FontWeight.350")
#define T_FONT_WEIGHT_400 T_("FontWeight.400")
#define T_FONT_WEIGHT_500 T_("FontWeight.500")
#define T_FONT_WEIGHT_600 T_("FontWeight.600")
#define T_FONT_WEIGHT_700 T_("FontWeight.700")
#define T_FONT_WEIGHT_800 T_("FontWeight.800")
#define T_FONT_WEIGHT_900 T_("FontWeight.900")
#define T_FONT_WEIGHT_950 T_("FontWeight.950")

#define T_FONT_STRETCH T_("FontStretch")
#define T_FONT_STRETCH_AUTO T_("FontStretch.Auto")
#define T_FONT_STRETCH_ULTRA_CONDENSED T_("FontStretch.UltraCondensed")
#define T_FONT_STRETCH_EXTRA_CONDENSED T_("FontStretch.ExtraCondensed")
#define T_FONT_STRETCH_CONDENSED T_("FontStretch.Condensed")
#define T_FONT_STRETCH_SEMI_CONDENSED T_("FontStretch.SemiCondensed")
#define T_FONT_STRETCH_NORMAL T_("FontStretch.Normal")
#define T_FONT_STRETCH_SEMI_EXPANDED T_("FontStretch.SemiExpanded")
#define T_FONT_STRETCH_EXPANDED T_("FontStretch.Expanded")
#define T_FONT_STRETCH_EXTRA_EXPANDED T_("FontStretch.ExtraExpanded")
#define T_FONT_STRETCH_ULTRA_EXPANDED T_("FontStretch.UltraExpanded")

#define T_HTML T_("Html")

#define T_ADVANCED T_("Advanced")

static char gradient_stop_color_names[MAX_GRADIENT_STOPS][32]{
	"gradient_color_0",  "gradient_color_1",  "gradient_color_2",  "gradient_color_3",  "gradient_color_4",
	"gradient_color_5",  "gradient_color_6",  "gradient_color_7",  "gradient_color_8",  "gradient_color_9",
	"gradient_color_10", "gradient_color_11", "gradient_color_12", "gradient_color_13", "gradient_color_14",
	"gradient_color_15",
};

//static char *gradient_stop_offset_names[MAX_GRADIENT_STOPS][32]{
//	"gradient_offset_0",  "gradient_offset_0",  "gradient_offset_0",  "gradient_offset_0",
//	"gradient_offset_0",  "gradient_offset_0",  "gradient_offset_0",  "gradient_offset_0",
//	"gradient_offset_0",  "gradient_offset_0",  "gradient_offset_0", "gradient_offset_0",
//	"gradient_offset_0", "gradient_offset_0", "gradient_offset_0", "gradient_offset_0",
//};

/* ------------------------------------------------------------------------- */

static inline DWORD get_alpha_val(uint32_t opacity)
{
	return ((opacity * 255 / 100) & 0xFF) << 24;
}

static inline DWORD calc_color(uint32_t color, uint32_t opacity)
{
	return color & 0xFFFFFF | get_alpha_val(opacity);
}

static inline std::wstring to_wide(const char *utf8)
{
	std::wstring text;

	size_t len = os_utf8_to_wcs(utf8, 0, nullptr, 0);
	text.resize(len);
	if (len)
		os_utf8_to_wcs(utf8, 0, &text[0], len + 1);

	return text;
}

static inline uint32_t rgb_to_bgr(uint32_t rgb)
{
	return ((rgb & 0xFF) << 16) | (rgb & 0xFF00) | ((rgb & 0xFF0000) >> 16);
}

static time_t get_modified_timestamp(const char *filename)
{
	struct stat stats;
	if (os_stat(filename, &stats) != 0)
		return -1;
	return stats.st_mtime;
}

void obs_dwrite_text_source::init_dwrite()
{
	if (gs_get_device_type() != GS_DEVICE_DIRECT3D_11) {
		error("Unsupported render backend! Are you using OpenGL?");
		throw winrt::hresult_error(CO_E_NOT_SUPPORTED);
	}

	winrt::check_hresult(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, pD2DFactory.put()));
	winrt::com_ptr<ID3D11Device> device((ID3D11Device *)gs_get_device_obj(),
					    winrt::take_ownership_from_abi);
	if (device == nullptr)
		throw winrt::hresult_error(E_UNEXPECTED);

	winrt::com_ptr<IDXGIDevice> dxgi = device.as<IDXGIDevice>();
	winrt::com_ptr<ID2D1Device> d2dDevice;
	winrt::check_hresult(pD2DFactory->CreateDevice(dxgi.get(), d2dDevice.put()));

	pD2DDevice = d2dDevice.as<ID2D1Device1>();
	winrt::check_hresult(
		pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, pD2DContext.put()));

	winrt::check_hresult(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory2),
						 reinterpret_cast<IUnknown **>(pDWriteFactory.put())));
}

void obs_dwrite_text_source::release() {}

void obs_dwrite_text_source::update_font() {}

gradient_axis_t obs_dwrite_text_source::calculate_gradient_axis(float width, float height)
{
	if (width <= 0.0f || height <= 0.0f)
		return {0.0f, 0.0f, 0.0f, 0.0f};

	float angle = (float)(atan(height / width) * 180.0f / M_PI);
	if (gradient_angle <= angle || gradient_angle > 360.0 - angle) {
		float y = (float)(width / 2 * tan(gradient_angle * M_PI / 180.0));
		return {width, height / 2.0f - y, 0, height / 2.0f + y};
	} else if (gradient_angle <= 180.0 - angle && gradient_angle > angle) {
		float x = (float)(height / 2 * tan((90.0 - gradient_angle) * M_PI / 180.0));
		return {width / 2.0f + x, 0, width / 2.0f - x, height};
	} else if (gradient_angle <= 180.0 + angle && gradient_angle > 180.0 - angle) {
		float y = (float)(width / 2 * tan(gradient_angle * M_PI / 180.0));
		return {0, height / 2.0f + y, width, height / 2.0f - y};
	} else {
		float x = (float)(height / 2 * tan((270.0 - gradient_angle) * M_PI / 180.0));
		return {width / 2.0f - x, height, width / 2.0f + x, 0.0f};
	}
}

bool obs_dwrite_text_source::create_render_target_d3d11(usize_t &size, thickness_t &padding)
{
	if (targetTexture)
		gs_texture_destroy(targetTexture);
	if (textTexture)
		gs_texture_destroy(textTexture);

	pTarget = nullptr;
	pTextTarget = nullptr;

	auto width = size.width;
	auto height = size.height;
	auto pad_width = (uint32_t)std::ceilf(std::clamp(size.width + (padding.left + padding.right),
							 (float)MIN_SIZE_CX, (float)MAX_SIZE_CX));
	auto pad_height = (uint32_t)std::ceilf(std::clamp(size.height + (padding.top + padding.bottom),
							  (float)MIN_SIZE_CY, (float)MAX_SIZE_CY));

	if (!use_extents) {
		textTexture = gs_texture_create(width, height, GS_BGRA_UNORM, 1, nullptr, GS_RENDER_TARGET);
	} else {
		textTexture =
			gs_texture_create(pad_width, pad_height, GS_BGRA_UNORM, 1, nullptr, GS_RENDER_TARGET);
	}

	targetTexture = gs_texture_create(pad_width, pad_height, GS_BGRA_UNORM, 1, nullptr, GS_RENDER_TARGET);

	if (!targetTexture || !textTexture)
		return false;

	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), 96, 96);

	winrt::com_ptr<ID3D11Texture2D> native_tex((ID3D11Texture2D *)gs_texture_get_obj(targetTexture),
						   winrt::take_ownership_from_abi);
	winrt::com_ptr<IDXGISurface> native_surface = native_tex.as<IDXGISurface>();
	HRESULT hr = pD2DContext->CreateBitmapFromDxgiSurface(native_surface.get(), &bitmapProperties,
							      pTarget.put());

	if (FAILED(hr) || pTarget == nullptr)
		return false;

	native_tex = winrt::com_ptr<ID3D11Texture2D>((ID3D11Texture2D *)gs_texture_get_obj(textTexture),
						     winrt::take_ownership_from_abi);
	native_surface = native_tex.as<IDXGISurface>();

	hr = pD2DContext->CreateBitmapFromDxgiSurface(native_surface.get(), &bitmapProperties,
						      pTextTarget.put());

	if (FAILED(hr) || pTextTarget == nullptr)
		return false;

	return true;
}

void obs_dwrite_text_source::update_brush(float width, float height)
{
	HRESULT hr;

	pFillBrush = nullptr;
	pOutlineBrush = nullptr;

	if (gradient_count > 1) {
		auto axis = calculate_gradient_axis(width, height);

		winrt::com_ptr<ID2D1GradientStopCollection> pGradientStops;

		float level = 1.0f / (gradient_count - 1);

		D2D1_GRADIENT_STOP *gradientStops = new D2D1_GRADIENT_STOP[gradient_count]{};
		for (size_t i = 0; i < gradient_count; i++) {
			gradientStops[i].color = D2D1::ColorF(gradient_stops[i].color, opacity / 100.0f);
			gradientStops[i].position = level * i;
		}

		hr = pD2DContext->CreateGradientStopCollection(gradientStops, (uint32_t)gradient_count,
							       D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_MIRROR,
							       pGradientStops.put());

		hr = pD2DContext->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(D2D1::Point2F(axis.x1, axis.y1),
							    D2D1::Point2F(axis.x2, axis.y2)),
			pGradientStops.get(), (ID2D1LinearGradientBrush **)pFillBrush.put());

	} else {
		hr = pD2DContext->CreateSolidColorBrush(D2D1::ColorF(gradient_stops[0].color, opacity / 100.0f),
							(ID2D1SolidColorBrush **)pFillBrush.put());
	}

	if (use_outline) {
		hr = pD2DContext->CreateSolidColorBrush(D2D1::ColorF(outline_color, outline_opacity / 100.0f),
							(ID2D1SolidColorBrush **)pOutlineBrush.put());
	}
}

void obs_dwrite_text_source::update_effects()
{
	pEffect = nullptr;

	if (use_shadow) {
		ComPtr<ID2D1Effect> shadowEffect;
		pD2DContext->CreateEffect(CLSID_D2D1Shadow, &shadowEffect);

		shadowEffect->SetInput(0, pTextTarget.get());
		shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, shadow_radius / 3.0f);
		shadowEffect->SetValue(D2D1_SHADOW_PROP_COLOR,
				       D2D1::ColorF(shadow_color, (shadow_opacity / 100.0f)));

		ComPtr<ID2D1Effect> affineTransformEffect;
		pD2DContext->CreateEffect(CLSID_D2D12DAffineTransform, &affineTransformEffect);

		affineTransformEffect->SetInputEffect(0, shadowEffect.Get());

		D2D1_MATRIX_3X2_F matrix = D2D1::Matrix3x2F::Translation(shadow_offset_x, shadow_offset_y);
		affineTransformEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, matrix);

		pD2DContext->CreateEffect(CLSID_D2D1Composite, pEffect.put());
		pEffect->SetInputEffect(0, affineTransformEffect.Get());
		pEffect->SetInput(1, pTextTarget.get());
	}
}

void obs_dwrite_text_source::draw_text()
{
	if (pTextRenderer)
		pTextRenderer = nullptr;

	pTextFormat = nullptr;
	pTextLayout = nullptr;

	auto weight = this->weight;
	auto stretch = this->stretch;
	auto style = this->style;

	UINT32 text_length = (UINT32)wcslen(text.c_str());
	HRESULT hr = S_OK;

	usize_t calculated_size = {0, 0};

	// calculate padding
	float top = 0, bottom = 0, left = 0, right = 0;
	if (use_shadow) {
		top = shadow_radius / 2.0f;
		bottom = shadow_radius / 2.0f;
		left = shadow_radius / 2.0f;
		right = shadow_radius / 2.0f;

		top -= shadow_offset_y;
		bottom += shadow_offset_y;
		left -= shadow_offset_x;
		right += shadow_offset_x;
	}

	thickness_t calculated_padding = {max(0, top), max(0, left), max(0, bottom), max(0, right)};

	if (use_outline) {
		calculated_padding.top += outline_size;
		calculated_padding.left += outline_size;
		calculated_padding.bottom += outline_size;
		calculated_padding.right += outline_size;
	}

	float layout_cx = (use_extents && extents_cx != -1)
				  ? (float)extents_cx - (calculated_padding.left + calculated_padding.right)
				  : INFINITY;
	float layout_cy = (use_extents && extents_cy != -1)
				  ? (float)extents_cy - (calculated_padding.top + calculated_padding.bottom)
				  : INFINITY;

	float text_cx = 0.0f;
	float text_cy = 0.0f;
	UINT32 lines = 1;

	WCHAR localeName[LOCALE_NAME_MAX_LENGTH] = {0};
	GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);

	std::wstring font_face = this->face;
	winrt::com_ptr<IDWriteGdiInterop> interop;
	winrt::com_ptr<IDWriteFont> font;
	winrt::com_ptr<IDWriteFontFamily> font_family;
	if (pDWriteFactory) {
		hr = pDWriteFactory->GetGdiInterop(interop.put());
	}

	if (SUCCEEDED(hr)) {
		LOGFONT lf = {};
		lf.lfCharSet = DEFAULT_CHARSET;
		if (!face.empty()) {
			wcscpy_s(lf.lfFaceName, face.c_str());
		}

		hr = interop->CreateFontFromLOGFONT(&lf, font.put());
	}

	if (SUCCEEDED(hr)) {
		hr = font->GetFontFamily(font_family.put());

		if (weight == S_FONT_WEIGHT_AUTO) {
			weight = font->GetWeight();
		}

		if (stretch == S_FONT_STRETCH_AUTO) {
			stretch = font->GetStretch();
		}
	}

	if (SUCCEEDED(hr)) {
		winrt::com_ptr<IDWriteLocalizedStrings> font_family_name;
		font_family->GetFamilyNames(font_family_name.put());

		UINT32 index = 0;
		UINT32 length = 0;
		BOOL exists = false;
		hr = font_family_name->FindLocaleName(localeName, &index, &exists);
		if (FAILED(hr) || !exists) {
			// fallback
			hr = font_family_name->FindLocaleName(L"en-us", &index, &exists);
		}

		if (SUCCEEDED(hr) && exists) {
			font_family_name->GetStringLength(index, &length);

			wchar_t *name = new wchar_t[length + 1];
			font_family_name->GetString(index, name, length + 1);
			font_face = std::wstring(name);
			delete[] name;
		}
	}

	if (SUCCEEDED(hr)) {
		hr = pDWriteFactory->CreateTextFormat(font_face.c_str(), NULL, weight, style, stretch,
						      (float)face_size / 96.0f * 72.0f, localeName,
						      pTextFormat.put());
	}

	if (SUCCEEDED(hr)) {
		pTextFormat->SetTextAlignment(align);
		pTextFormat->SetParagraphAlignment(valign);
		pTextFormat->SetWordWrapping(wrap);

		hr = pDWriteFactory->CreateTextLayout(text.c_str(), text_length, pTextFormat.get(), layout_cx,
						      layout_cy, pTextLayout.put());
	}

	if (SUCCEEDED(hr)) {
		DWRITE_TEXT_RANGE text_range = {0, text_length};
		pTextLayout->SetUnderline(underline, text_range);
		pTextLayout->SetStrikethrough(strikeout, text_range);
		pTextLayout->SetWordWrapping(wrap);

		for (auto &&run : runs) {
			DWRITE_TEXT_RANGE run_range = {run.start, run.length};

			if (run.format == format_flags::bold) {
				pTextLayout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, run_range);
			}

			if (run.format == format_flags::italic) {
				pTextLayout->SetFontStyle(DWRITE_FONT_STYLE_ITALIC, run_range);
			}

			if (run.format == format_flags::underline) {
				pTextLayout->SetUnderline(true, run_range);
			}

			if (run.format == format_flags::strikethrough) {
				pTextLayout->SetStrikethrough(true, run_range);
			}

			if (run.size.has_value()) {
				pTextLayout->SetFontSize(run.size.value(), run_range);
			}
		}

		if (text_trimming != S_TRIMMING_NONE) {
			ComPtr<IDWriteInlineObject> inlineObject;
			pDWriteFactory->CreateEllipsisTrimmingSign(pTextFormat.get(),
								   inlineObject.GetAddressOf());

			DWRITE_TRIMMING trimming = {(text_trimming == S_TRIMMING_CHARACTER_ELLIPSIS
							     ? DWRITE_TRIMMING_GRANULARITY_CHARACTER
							     : DWRITE_TRIMMING_GRANULARITY_WORD),
						    0, 0};

			pTextLayout->SetTrimming(&trimming, inlineObject.Get());
		} else {
			DWRITE_TRIMMING trimming = {DWRITE_TRIMMING_GRANULARITY_NONE, 0, 0};
			pTextLayout->SetTrimming(&trimming, nullptr);
		}

		pTextLayout->SetMaxWidth(layout_cx);
		pTextLayout->SetMaxHeight(layout_cy);
	}

	// calculate size
	if (SUCCEEDED(hr)) {
		DWRITE_TEXT_METRICS textMetrics;
		hr = pTextLayout->GetMetrics(&textMetrics);

		text_cx = ceil(textMetrics.widthIncludingTrailingWhitespace);
		text_cy = ceil(textMetrics.height);
		lines = textMetrics.lineCount;

		if (isnan(text_cx))
			text_cx = MAX_SIZE_CX;
		if (isnan(text_cy))
			text_cy = MAX_SIZE_CY;

		if (!use_extents || extents_cx == -1) {
			layout_cx = text_cx;
		}

		if (!use_extents || extents_cy == -1) {
			layout_cy = text_cy;
		}

		clamp_assign(layout_cx, MIN_SIZE_CX, MAX_SIZE_CX);
		clamp_assign(layout_cy, MIN_SIZE_CY, MAX_SIZE_CY);

		calculated_size = {(uint32_t)layout_cx, (uint32_t)layout_cy};
	}

	if (!pTarget || !targetTexture || size != calculated_size || calculated_padding != padding) {
		obs_enter_graphics();

		if (!create_render_target_d3d11(calculated_size, calculated_padding))
			return;

		obs_leave_graphics();

		size = calculated_size;
		padding = calculated_padding;
	}

	if (SUCCEEDED(hr)) {
		update_brush(text_cx, text_cy / lines);

		pTextRenderer.attach(new OBSTextRenderer(pDWriteFactory.get(), pD2DFactory.get(),
							 pD2DContext.get(), pOutlineBrush.get(),
							 pFillBrush.get(), outline_size, color_fonts));

		pD2DContext->BeginDraw();
		pD2DContext->SetTarget(pTextTarget.get());
		pD2DContext->SetTransform(D2D1::IdentityMatrix());
		pD2DContext->Clear(D2D1::ColorF(0, 0, 0, 0));

		pD2DContext->SetTextAntialiasMode(antialias ? D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE
							    : D2D1_TEXT_ANTIALIAS_MODE_ALIASED);

		pTextLayout->Draw(NULL, pTextRenderer.get(), 0, 0);

		hr = pD2DContext->EndDraw();

		update_effects();

		pD2DContext->BeginDraw();
		pD2DContext->SetTarget(pTarget.get());
		pD2DContext->SetTransform(D2D1::IdentityMatrix());
		pD2DContext->Clear(D2D1::ColorF(bk_color, bk_opacity / 100.0f));

		if (pEffect) {
			pD2DContext->DrawImage(pEffect.get(), D2D1::Point2F(padding.left, padding.top));
		} else {
			pD2DContext->DrawImage(pTextTarget.get(), D2D1::Point2F(padding.left, padding.top));
		}

		hr = pD2DContext->EndDraw();
	}
}

void obs_dwrite_text_source::transform_text()
{
	const std::locale loc = std::locale(obs_get_locale());
	const std::ctype<wchar_t> &f = std::use_facet<std::ctype<wchar_t>>(loc);
	if (text_transform == S_TRANSFORM_UPPERCASE)
		f.toupper(&text[0], &text[0] + text.size());
	else if (text_transform == S_TRANSFORM_LOWERCASE)
		f.tolower(&text[0], &text[0] + text.size());
	else if (text_transform == S_TRANSFORM_STARTCASE) {
		bool upper = true;
		for (auto it = text.begin(); it != text.end(); ++it) {
			const wchar_t upper_char = f.toupper(*it);
			const wchar_t lower_char = f.tolower(*it);
			if (upper && lower_char != upper_char) {
				upper = false;
				*it = upper_char;
			} else if (lower_char != upper_char) {
				*it = lower_char;
			} else {
				upper = iswspace(*it);
			}
		}
	}

	runs.clear();
}

const char *obs_dwrite_text_source::get_string(const char *str)
{
	if (!str)
		return "";
	if (!chatlog_mode || !chatlog_lines)
		return str;

	int lines = chatlog_lines;
	size_t len = strlen(str);
	if (!len)
		return str;

	const char *temp = str + len;

	while (temp != str) {
		temp--;

		if (temp[0] == '\n' && temp[1] != 0) {
			if (!--lines)
				break;
		}
	}

	return *temp == '\n' ? temp + 1 : temp;
}

void obs_dwrite_text_source::load_file()
{
	BPtr<char> file_text = os_quick_read_utf8_file(file.c_str());
	text = to_wide(get_string(file_text));
}

#define obs_data_get_uint32 (uint32_t) obs_data_get_int
#define obs_data_get_int32 (int32_t) obs_data_get_int

inline void obs_dwrite_text_source::Update(obs_data_t *s)
{
	if (!pD2DContext)
		return;

	const char *new_text = obs_data_get_string(s, S_TEXT);
	obs_data_t *font_obj = obs_data_get_obj(s, S_FONT);
	const char *align_str = obs_data_get_string(s, S_ALIGN);
	const char *valign_str = obs_data_get_string(s, S_VALIGN);
	const char *wrap_str = obs_data_get_string(s, S_WRAP_MODE);
	uint32_t new_opacity = obs_data_get_uint32(s, S_OPACITY);

	bool new_outline = obs_data_get_bool(s, S_OUTLINE);
	uint32_t new_o_color = obs_data_get_uint32(s, S_OUTLINE_COLOR);
	uint32_t new_o_opacity = obs_data_get_uint32(s, S_OUTLINE_OPACITY);
	uint32_t new_o_size = obs_data_get_uint32(s, S_OUTLINE_SIZE);

	bool new_use_file = obs_data_get_bool(s, S_USE_FILE);
	const char *new_file = obs_data_get_string(s, S_FILE);

	bool new_chat_mode = obs_data_get_bool(s, S_CHATLOG_MODE);
	int32_t new_chat_lines = obs_data_get_int32(s, S_CHATLOG_LINES);

	bool new_extents = obs_data_get_bool(s, S_EXTENTS);
	int32_t n_extents_cx = (int32_t)obs_data_get_int(s, S_EXTENTS_CX);
	int32_t n_extents_cy = (int32_t)obs_data_get_int(s, S_EXTENTS_CY);

	const char *font_face = obs_data_get_string(font_obj, S_FONT_FACE);
	int32_t font_size = obs_data_get_int32(font_obj, S_FONT_SIZE);
	int64_t font_flags = obs_data_get_int(font_obj, S_FONT_FLAGS);
	//bool new_bold = (font_flags & OBS_FONT_BOLD) != 0;
	bool new_italic = (font_flags & OBS_FONT_ITALIC) != 0;
	bool new_underline = (font_flags & OBS_FONT_UNDERLINE) != 0;
	bool new_strikeout = (font_flags & OBS_FONT_STRIKEOUT) != 0;

	bool new_color_fonts = obs_data_get_bool(s, S_COLOR_FONTS);
	bool new_antialias = obs_data_get_bool(s, S_ANTIALIASING);

	int32_t new_text_transform = obs_data_get_int32(s, S_TRANSFORM);
	int32_t new_text_trimming = obs_data_get_int32(s, S_TRIMMING);

	int32_t new_font_weight = obs_data_get_int32(s, S_FONT_WEIGHT);
	int32_t new_font_stretch = obs_data_get_int32(s, S_FONT_STRETCH);

	uint32_t new_bk_color = obs_data_get_uint32(s, S_BKCOLOR);
	uint32_t new_bk_opacity = obs_data_get_uint32(s, S_BKOPACITY);

	int32_t new_gradient_count = obs_data_get_int32(s, S_GRADIENT_EX_COUNT);
	float new_grad_dir = (float)obs_data_get_double(s, S_GRADIENT_DIR);

	bool new_shadow = obs_data_get_bool(s, S_SHADOW);
	float new_shadow_offset_x = (float)obs_data_get_double(s, S_SHADOW_OFFSET_X);
	float new_shadow_offset_y = (float)obs_data_get_double(s, S_SHADOW_OFFSET_Y);
	float new_shadow_radius = (float)obs_data_get_double(s, S_SHADOW_RADIUS);
	uint32_t new_shadow_color = obs_data_get_uint32(s, S_SHADOW_COLOR);
	uint32_t new_shadow_opacity = obs_data_get_uint32(s, S_SHADOW_OPACITY);

	/* ----------------------------- */

	std::wstring new_face = to_wide(font_face);

	if (new_face != face || face_size != font_size || new_font_weight != weight || new_italic != italic ||
	    new_underline != underline || new_strikeout != strikeout || new_font_stretch != stretch) {

		face = new_face;
		face_size = font_size;
		weight = (DWRITE_FONT_WEIGHT)new_font_weight;
		italic = new_italic;
		underline = new_underline;
		strikeout = new_strikeout;
		stretch = (DWRITE_FONT_STRETCH)new_font_stretch;

		if (italic)
			style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC;
		else
			style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
	}

	/* ----------------------------- */
	new_o_color = rgb_to_bgr(new_o_color);
	new_bk_color = rgb_to_bgr(new_bk_color);
	new_shadow_color = rgb_to_bgr(new_shadow_color);

	opacity = new_opacity;
	gradient_angle = new_grad_dir;
	color_fonts = new_color_fonts;
	antialias = new_antialias;
	text_transform = new_text_transform;
	text_trimming = new_text_trimming;

	gradient_count = new_gradient_count;
	for (size_t i = 0; i < new_gradient_count; i++) {
		gradient_stops[i].color = rgb_to_bgr(obs_data_get_uint32(s, gradient_stop_color_names[i]));
	}

	bk_color = new_bk_color;
	bk_opacity = new_bk_opacity;
	use_extents = new_extents;
	extents_cx = n_extents_cx;
	extents_cy = n_extents_cy;

	read_from_file = new_use_file;

	chatlog_mode = new_chat_mode;
	chatlog_lines = new_chat_lines;

	if (read_from_file) {
		file = new_file;
		file_timestamp = get_modified_timestamp(new_file);
		load_file();

	} else {
		text = to_wide(get_string(new_text));
	}

	transform_text();

	use_outline = new_outline;
	outline_color = new_o_color;
	outline_opacity = new_o_opacity;
	outline_size = roundf(float(new_o_size));

	use_shadow = new_shadow;
	shadow_color = new_shadow_color;
	shadow_offset_x = new_shadow_offset_x;
	shadow_offset_y = new_shadow_offset_y;
	shadow_radius = new_shadow_radius;
	shadow_opacity = new_shadow_opacity;

	if (strcmp(align_str, S_ALIGN_CENTER) == 0)
		align = DWRITE_TEXT_ALIGNMENT_CENTER;
	else if (strcmp(align_str, S_ALIGN_RIGHT) == 0)
		align = DWRITE_TEXT_ALIGNMENT_TRAILING;
	else if (strcmp(align_str, S_ALIGN_JUSTIFIED) == 0)
		align = DWRITE_TEXT_ALIGNMENT_JUSTIFIED;
	else
		align = DWRITE_TEXT_ALIGNMENT_LEADING;

	if (strcmp(valign_str, S_VALIGN_CENTER) == 0)
		valign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
	else if (strcmp(valign_str, S_VALIGN_BOTTOM) == 0)
		valign = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
	else
		valign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;

	if (strcmp(wrap_str, S_WRAP_MODE_WRAP_WHOLE_WORDS) == 0)
		wrap = DWRITE_WORD_WRAPPING_WHOLE_WORD;
	else if (strcmp(wrap_str, S_WRAP_MODE_WRAP_CHARACTER) == 0)
		wrap = DWRITE_WORD_WRAPPING_CHARACTER;
	else if (strcmp(wrap_str, S_WRAP_MODE_WRAP) == 0)
		wrap = DWRITE_WORD_WRAPPING_WRAP;
	else if (strcmp(wrap_str, S_WRAP_MODE_NONE) == 0)
		wrap = DWRITE_WORD_WRAPPING_NO_WRAP;

	draw_text();
	update_time_elapsed = 0.0f;

	/* ----------------------------- */

	obs_data_release(font_obj);
}

inline void obs_dwrite_text_source::Tick(float seconds)
{
	if (!pD2DContext)
		return;

	if (!read_from_file)
		return;

	update_time_elapsed += seconds;

	if (update_time_elapsed >= 1.0f) {
		time_t t = get_modified_timestamp(file.c_str());
		update_time_elapsed = 0.0f;

		if (file_timestamp != t) {
			load_file();
			transform_text();
			draw_text();
			file_timestamp = t;
		}
	}
}

inline void obs_dwrite_text_source::Render(gs_effect_t *)
{
	if (!pD2DContext)
		return;

	if (!targetTexture)
		return;

	gs_texture_t *texture = targetTexture;
	gs_effect_t *effect = obs_get_base_effect(OBS_EFFECT_DEFAULT);
	gs_technique_t *tech = gs_effect_get_technique(effect, "Draw");
	gs_eparam_t *image = gs_effect_get_param_by_name(effect, "image");

	gs_blend_state_push();
	gs_blend_function(GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);

	const bool previous = gs_framebuffer_srgb_enabled();
	gs_enable_framebuffer_srgb(false);

	size_t passes = gs_technique_begin(tech);

	for (size_t i = 0; i < passes; i++) {
		if (gs_technique_begin_pass(tech, i)) {
			gs_effect_set_texture(image, texture);
			gs_draw_sprite(texture, 0, 0, 0);
			gs_technique_end_pass(tech);
		}
	}

	gs_technique_end(tech);

	gs_enable_framebuffer_srgb(previous);
	gs_blend_state_pop();
}

/* ------------------------------------------------------------------------- */

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("text-directwrite", "en-US")
MODULE_EXPORT const char *obs_module_description(void)
{
	return "Windows DirectWrite/Direct2D text source";
}

/* clang-format off */

#define set_vis(var, val, show)						\
	do {											\
		p = obs_properties_get(props, val);			\
		obs_property_set_visible(p, var == show);	\
	} while (false)

/* clang-format on */

static bool use_file_changed(obs_properties_t *props, obs_property_t *p, obs_data_t *s)
{
	bool use_file = obs_data_get_bool(s, S_USE_FILE);

	set_vis(use_file, S_TEXT, false);
	set_vis(use_file, S_FILE, true);
	return true;
}

static bool outline_changed(obs_properties_t *props, obs_property_t *p, obs_data_t *s)
{
	bool outline = obs_data_get_bool(s, S_OUTLINE);

	set_vis(outline, S_OUTLINE_SIZE, true);
	set_vis(outline, S_OUTLINE_COLOR, true);
	set_vis(outline, S_OUTLINE_OPACITY, true);
	return true;
}

static bool shadow_changed(obs_properties_t *props, obs_property_t *p, obs_data_t *s)
{
	bool outline = obs_data_get_bool(s, S_SHADOW);

	set_vis(outline, S_SHADOW_RADIUS, true);
	set_vis(outline, S_SHADOW_COLOR, true);
	set_vis(outline, S_SHADOW_OFFSET_X, true);
	set_vis(outline, S_SHADOW_OFFSET_Y, true);
	set_vis(outline, S_SHADOW_OPACITY, true);
	return true;
}

static bool chatlog_mode_changed(obs_properties_t *props, obs_property_t *p, obs_data_t *s)
{
	bool chatlog_mode = obs_data_get_bool(s, S_CHATLOG_MODE);

	set_vis(chatlog_mode, S_CHATLOG_LINES, true);
	return true;
}

static bool gradient_changed(obs_properties_t *props, obs_property_t *p, obs_data_t *s)
{
	int32_t gradient_stops = max(1, min(obs_data_get_int32(s, S_GRADIENT_EX_COUNT), MAX_GRADIENT_STOPS));
	for (size_t i = 0; i < MAX_GRADIENT_STOPS; i++) {
		set_vis(i < gradient_stops, gradient_stop_color_names[i], true);
		//set_vis(i < gradient_stops, gradient_stop_offset_names[i], true);
	}

	return true;
}

static bool extents_modified(obs_properties_t *props, obs_property_t *p, obs_data_t *s)
{
	bool use_extents = obs_data_get_bool(s, S_EXTENTS);

	set_vis(use_extents, S_EXTENTS_CX, true);
	set_vis(use_extents, S_EXTENTS_CY, true);
	return true;
}

#undef set_vis

static obs_properties_t *get_properties(void *data)
{
	obs_dwrite_text_source *s = reinterpret_cast<obs_dwrite_text_source *>(data);
	std::string path;

	obs_properties_t *props = obs_properties_create();
	obs_property_t *p;

	std::string filter;
	filter += T_FILTER_TEXT_FILES;
	filter += " (*.txt);;";
	filter += T_FILTER_ALL_FILES;
	filter += " (*.*)";

	if (s && !s->file.empty()) {
		const char *slash;

		path = s->file;
		replace(path.begin(), path.end(), '\\', '/');
		slash = strrchr(path.c_str(), '/');
		if (slash)
			path.resize(slash - path.c_str() + 1);
	}

	// text group
	obs_properties_t *text_group = obs_properties_create();
	obs_properties_add_group(props, S_TEXT_GROUP, T_TEXT, OBS_GROUP_NORMAL, text_group);

	obs_properties_add_text(text_group, S_TEXT, T_TEXT, OBS_TEXT_MULTILINE);
	obs_properties_add_path(text_group, S_FILE, T_FILE, OBS_PATH_FILE, filter.c_str(), path.c_str());

	p = obs_properties_add_bool(text_group, S_USE_FILE, T_USE_FILE);
	obs_property_set_modified_callback(p, use_file_changed);

	// font group
	obs_properties_t *font_group = obs_properties_create();
	obs_properties_add_group(props, S_FONT_GROUP, T_FONT, OBS_GROUP_NORMAL, font_group);

	obs_properties_add_font(font_group, S_FONT, T_FONT);

	p = obs_properties_add_list(font_group, S_FONT_WEIGHT, T_FONT_WEIGHT, OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, T_FONT_WEIGHT_AUTO, S_FONT_WEIGHT_AUTO);
	obs_property_list_add_int(p, T_FONT_WEIGHT_100, S_FONT_WEIGHT_100);
	obs_property_list_add_int(p, T_FONT_WEIGHT_200, S_FONT_WEIGHT_200);
	obs_property_list_add_int(p, T_FONT_WEIGHT_300, S_FONT_WEIGHT_300);
	obs_property_list_add_int(p, T_FONT_WEIGHT_350, S_FONT_WEIGHT_350);
	obs_property_list_add_int(p, T_FONT_WEIGHT_400, S_FONT_WEIGHT_400);
	obs_property_list_add_int(p, T_FONT_WEIGHT_500, S_FONT_WEIGHT_500);
	obs_property_list_add_int(p, T_FONT_WEIGHT_600, S_FONT_WEIGHT_600);
	obs_property_list_add_int(p, T_FONT_WEIGHT_700, S_FONT_WEIGHT_700);
	obs_property_list_add_int(p, T_FONT_WEIGHT_800, S_FONT_WEIGHT_800);
	obs_property_list_add_int(p, T_FONT_WEIGHT_900, S_FONT_WEIGHT_900);
	obs_property_list_add_int(p, T_FONT_WEIGHT_950, S_FONT_WEIGHT_950);

	p = obs_properties_add_list(font_group, S_FONT_STRETCH, T_FONT_STRETCH, OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, T_FONT_STRETCH_AUTO, S_FONT_STRETCH_AUTO);
	obs_property_list_add_int(p, T_FONT_STRETCH_ULTRA_CONDENSED, S_FONT_STRETCH_ULTRA_CONDENSED);
	obs_property_list_add_int(p, T_FONT_STRETCH_EXTRA_CONDENSED, S_FONT_STRETCH_EXTRA_CONDENSED);
	obs_property_list_add_int(p, T_FONT_STRETCH_CONDENSED, S_FONT_STRETCH_CONDENSED);
	obs_property_list_add_int(p, T_FONT_STRETCH_SEMI_CONDENSED, S_FONT_STRETCH_SEMI_CONDENSED);
	obs_property_list_add_int(p, T_FONT_STRETCH_NORMAL, S_FONT_STRETCH_NORMAL);
	obs_property_list_add_int(p, T_FONT_STRETCH_SEMI_EXPANDED, S_FONT_STRETCH_SEMI_EXPANDED);
	obs_property_list_add_int(p, T_FONT_STRETCH_EXPANDED, S_FONT_STRETCH_EXPANDED);
	obs_property_list_add_int(p, T_FONT_STRETCH_EXTRA_EXPANDED, S_FONT_STRETCH_EXTRA_EXPANDED);
	obs_property_list_add_int(p, T_FONT_STRETCH_ULTRA_EXPANDED, S_FONT_STRETCH_ULTRA_EXPANDED);

	// colours/gradient group
	obs_properties_t *colors_group = obs_properties_create();
	obs_properties_add_group(props, S_GRADIENT_EX, T_COLOR, OBS_GROUP_NORMAL, colors_group);

	p = obs_properties_add_int_slider(colors_group, S_GRADIENT_EX_COUNT, T_COLORS, 1, MAX_GRADIENT_STOPS,
					  1);
	obs_property_set_modified_callback(p, gradient_changed);

	for (size_t i = 0; i < MAX_GRADIENT_STOPS; i++) {
		obs_properties_add_color(colors_group, gradient_stop_color_names[i], T_COLOR);
		//obs_properties_add_float_slider(colors, gradient_stop_offset_names[i], T_OFFSET, 0, 1, 0.01);
	}

	obs_properties_add_int_slider(colors_group, S_OPACITY, T_OPACITY, 0, 100, 1);
	obs_properties_add_float_slider(colors_group, S_GRADIENT_DIR, T_GRADIENT_DIR, 0, 360, 0.1);

	// background colour/opacity group
	obs_properties_t *background_group = obs_properties_create();
	obs_properties_add_group(props, S_BKGROUP, T_BKGROUP, OBS_GROUP_NORMAL, background_group);

	obs_properties_add_color(background_group, S_BKCOLOR, T_BKCOLOR);
	obs_properties_add_int_slider(background_group, S_BKOPACITY, T_BKOPACITY, 0, 100, 1);

	// layout group
	obs_properties_t *layout_group = obs_properties_create();
	obs_properties_add_group(props, S_LAYOUT, T_LAYOUT, OBS_GROUP_NORMAL, layout_group);

	p = obs_properties_add_list(layout_group, S_ALIGN, T_ALIGN, OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, T_ALIGN_LEFT, S_ALIGN_LEFT);
	obs_property_list_add_string(p, T_ALIGN_CENTER, S_ALIGN_CENTER);
	obs_property_list_add_string(p, T_ALIGN_RIGHT, S_ALIGN_RIGHT);
	obs_property_list_add_string(p, T_ALIGN_JUSTIFIED, S_ALIGN_JUSTIFIED);

	p = obs_properties_add_list(layout_group, S_VALIGN, T_VALIGN, OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, T_VALIGN_TOP, S_VALIGN_TOP);
	obs_property_list_add_string(p, T_VALIGN_CENTER, S_VALIGN_CENTER);
	obs_property_list_add_string(p, T_VALIGN_BOTTOM, S_VALIGN_BOTTOM);

	p = obs_properties_add_list(layout_group, S_WRAP_MODE, T_WRAP_MODE, OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, T_WRAP_MODE_NONE, S_WRAP_MODE_NONE);
	obs_property_list_add_string(p, T_WRAP_MODE_WRAP, S_WRAP_MODE_WRAP);
	obs_property_list_add_string(p, T_WRAP_MODE_WRAP_CHARACTER, S_WRAP_MODE_WRAP_CHARACTER);
	obs_property_list_add_string(p, T_WRAP_MODE_WRAP_WHOLE_WORDS, S_WRAP_MODE_WRAP_WHOLE_WORDS);

	p = obs_properties_add_list(layout_group, S_TRIMMING, T_TRIMMING, OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, T_TRIMMING_NONE, S_TRIMMING_NONE);
	obs_property_list_add_int(p, T_TRIMMING_CHARACTER_ELLIPSIS, S_TRIMMING_CHARACTER_ELLIPSIS);
	obs_property_list_add_int(p, T_TRIMMING_WORD_ELLIPSIS, S_TRIMMING_WORD_ELLIPSIS);

	p = obs_properties_add_list(layout_group, S_TRANSFORM, T_TRANSFORM, OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, T_TRANSFORM_NONE, S_TRANSFORM_NONE);
	obs_property_list_add_int(p, T_TRANSFORM_UPPERCASE, S_TRANSFORM_UPPERCASE);
	obs_property_list_add_int(p, T_TRANSFORM_LOWERCASE, S_TRANSFORM_LOWERCASE);
	obs_property_list_add_int(p, T_TRANSFORM_STARTCASE, S_TRANSFORM_STARTCASE);

	p = obs_properties_add_bool(layout_group, S_EXTENTS, T_EXTENTS);
	obs_property_set_modified_callback(p, extents_modified);

	obs_properties_add_int(layout_group, S_EXTENTS_CX, T_EXTENTS_CX, -1, 8000, 1);
	obs_properties_add_int(layout_group, S_EXTENTS_CY, T_EXTENTS_CY, -1, 8000, 1);

	// shadow group
	obs_properties_t *shadow_group = obs_properties_create();
	obs_properties_add_group(props, S_SHADOW_GROUP, T_SHADOW, OBS_GROUP_NORMAL, shadow_group);

	p = obs_properties_add_bool(shadow_group, S_SHADOW, T_SHADOW);
	obs_property_set_modified_callback(p, shadow_changed);

	obs_properties_add_float(shadow_group, S_SHADOW_OFFSET_X, T_SHADOW_OFFSET_X, 0, 64, 2);
	obs_properties_add_float(shadow_group, S_SHADOW_OFFSET_Y, T_SHADOW_OFFSET_Y, 0, 64, 2);
	obs_properties_add_float(shadow_group, S_SHADOW_RADIUS, T_SHADOW_RADIUS, 0, 100, 2);
	obs_properties_add_color(shadow_group, S_SHADOW_COLOR, T_SHADOW_COLOR);
	obs_properties_add_int_slider(shadow_group, S_SHADOW_OPACITY, T_SHADOW_OPACITY, 0, 100, 1);

	// outline group
	obs_properties_t *outline_group = obs_properties_create();
	obs_properties_add_group(props, S_OUTLINE_GROUP, T_OUTLINE, OBS_GROUP_NORMAL, outline_group);

	p = obs_properties_add_bool(outline_group, S_OUTLINE, T_OUTLINE);
	obs_property_set_modified_callback(p, outline_changed);

	obs_properties_add_int(outline_group, S_OUTLINE_SIZE, T_OUTLINE_SIZE, 1, 20, 1);
	obs_properties_add_color(outline_group, S_OUTLINE_COLOR, T_OUTLINE_COLOR);
	obs_properties_add_int_slider(outline_group, S_OUTLINE_OPACITY, T_OUTLINE_OPACITY, 0, 100, 1);

	// advanced group
	obs_properties_t *advanced_group = obs_properties_create();
	obs_properties_add_group(props, S_ADVANCED, T_ADVANCED, OBS_GROUP_NORMAL, advanced_group);

	p = obs_properties_add_bool(advanced_group, S_CHATLOG_MODE, T_CHATLOG_MODE);
	obs_property_set_modified_callback(p, chatlog_mode_changed);

	obs_properties_add_int(advanced_group, S_CHATLOG_LINES, T_CHATLOG_LINES, 1, 1000, 1);

	p = obs_properties_add_bool(advanced_group, S_ANTIALIASING, T_ANTIALIASING);
	p = obs_properties_add_bool(advanced_group, S_COLOR_FONTS, T_COLOR_FONTS);

	return props;
}

bool obs_module_load(void)
{
	obs_source_info si = {};
	si.id = "text_directwrite";
	si.type = OBS_SOURCE_TYPE_INPUT;
	si.icon_type = OBS_ICON_TYPE_TEXT;
	si.output_flags = OBS_SOURCE_VIDEO;
	si.get_properties = get_properties;
	si.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW | OBS_SOURCE_SRGB;

	si.get_name = [](void *) { return obs_module_text("TextDirectWrite"); };
	si.create = [](obs_data_t *settings, obs_source_t *source) {
		return (void *)new obs_dwrite_text_source(source, settings);
	};
	si.destroy = [](void *data) { delete reinterpret_cast<obs_dwrite_text_source *>(data); };
	si.get_width = [](void *data) { return reinterpret_cast<obs_dwrite_text_source *>(data)->get_width(); };
	si.get_height = [](void *data) {
		return reinterpret_cast<obs_dwrite_text_source *>(data)->get_height();
	};
	si.get_defaults = [](obs_data_t *settings) {
		obs_data_t *font_obj = obs_data_create();
		obs_data_set_default_string(font_obj, S_FONT_FACE, "Arial");
		obs_data_set_default_int(font_obj, S_FONT_SIZE, 256);

		obs_data_set_default_obj(settings, S_FONT, font_obj);
		obs_data_set_default_string(settings, S_ALIGN, S_ALIGN_LEFT);
		obs_data_set_default_string(settings, S_VALIGN, S_VALIGN_TOP);
		obs_data_set_default_string(settings, S_WRAP_MODE, S_WRAP_MODE_WRAP);
		obs_data_set_default_int(settings, S_FONT_WEIGHT, S_FONT_WEIGHT_AUTO);
		obs_data_set_default_int(settings, S_FONT_STRETCH, S_FONT_STRETCH_AUTO);
		obs_data_set_default_int(settings, S_COLOR, 0xFFFFFF);
		obs_data_set_default_int(settings, S_GRADIENT_EX_COUNT, 1);
		obs_data_set_default_int(settings, S_OPACITY, 100);
		obs_data_set_default_double(settings, S_GRADIENT_DIR, 90.0);
		obs_data_set_default_int(settings, S_BKCOLOR, 0x000000);
		obs_data_set_default_int(settings, S_BKOPACITY, 0);
		obs_data_set_default_int(settings, S_OUTLINE_SIZE, 2);
		obs_data_set_default_int(settings, S_OUTLINE_COLOR, 0xFFFFFF);
		obs_data_set_default_int(settings, S_OUTLINE_OPACITY, 100);
		obs_data_set_default_int(settings, S_CHATLOG_LINES, 6);
		obs_data_set_default_int(settings, S_SHADOW_RADIUS, 4);
		obs_data_set_default_int(settings, S_SHADOW_COLOR, 0x000000);
		obs_data_set_default_int(settings, S_SHADOW_OPACITY, 50);
		obs_data_set_default_int(settings, S_EXTENTS_CX, 100);
		obs_data_set_default_int(settings, S_EXTENTS_CY, 100);
		obs_data_set_default_bool(settings, S_COLOR_FONTS, true);
		obs_data_set_default_bool(settings, S_ANTIALIASING, true);

		obs_data_release(font_obj);
	};
	si.update = [](void *data, obs_data_t *settings) {
		reinterpret_cast<obs_dwrite_text_source *>(data)->Update(settings);
	};
	si.video_tick = [](void *data, float seconds) {
		reinterpret_cast<obs_dwrite_text_source *>(data)->Tick(seconds);
	};
	si.video_render = [](void *data, gs_effect_t *effect) {
		reinterpret_cast<obs_dwrite_text_source *>(data)->Render(effect);
	};

	obs_register_source(&si);

	return true;
}

void obs_module_unload(void) {}
