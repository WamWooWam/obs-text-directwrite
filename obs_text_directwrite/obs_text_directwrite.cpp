
#include "obs_text_directwrite.h"

/* ------------------------------------------------------------------------- */

#define S_FONT "font"
#define S_USE_FILE "read_from_file"
#define S_FILE "file"
#define S_TEXT "text"
#define S_COLOR "color"
#define S_GRADIENT "gradient"
#define S_GRADIENT_NONE "none"
#define S_GRADIENT_TWO "two_colors"
#define S_GRADIENT_THREE "three_colors"
#define S_GRADIENT_FOUR "four_colors"
#define S_GRADIENT_COLOR "gradient_color"
#define S_GRADIENT_COLOR2 "gradient_color2"
#define S_GRADIENT_COLOR3 "gradient_color3"
#define S_GRADIENT_DIR "gradient_dir"
#define S_GRADIENT_OPACITY "gradient_opacity"
#define S_ALIGN "align"
#define S_VALIGN "valign"
#define S_OPACITY "opacity"
#define S_BKCOLOR "bk_color"
#define S_BKOPACITY "bk_opacity"
//#define S_VERTICAL                      "vertical"
#define S_OUTLINE "outline"
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
#define S_FONT_STRETCH_ULTRA_CONDENSED DWRITE_FONT_STRETCH_ULTRA_CONDENSED
#define S_FONT_STRETCH_EXTRA_CONDENSED DWRITE_FONT_STRETCH_EXTRA_CONDENSED
#define S_FONT_STRETCH_CONDENSED DWRITE_FONT_STRETCH_CONDENSED
#define S_FONT_STRETCH_SEMI_CONDENSED DWRITE_FONT_STRETCH_SEMI_CONDENSED
#define S_FONT_STRETCH_NORMAL DWRITE_FONT_STRETCH_NORMAL
#define S_FONT_STRETCH_SEMI_EXPANDED DWRITE_FONT_STRETCH_SEMI_EXPANDED
#define S_FONT_STRETCH_EXPANDED DWRITE_FONT_STRETCH_EXPANDED
#define S_FONT_STRETCH_EXTRA_EXPANDED DWRITE_FONT_STRETCH_EXTRA_EXPANDED
#define S_FONT_STRETCH_ULTRA_EXPANDED DWRITE_FONT_STRETCH_ULTRA_EXPANDED

#define S_HTML "html"

#define T_(v) obs_module_text(v)
#define T_FONT T_("Font")
#define T_USE_FILE T_("ReadFromFile")
#define T_FILE T_("TextFile")
#define T_TEXT T_("Text")
#define T_COLOR T_("Color")
#define T_GRADIENT T_("Gradient")
#define T_GRADIENT_NONE T_("Gradient.None")
#define T_GRADIENT_TWO T_("Gradient.TwoColors")
#define T_GRADIENT_THREE T_("Gradient.ThreeColors")
#define T_GRADIENT_FOUR T_("Gradient.FourColors")
#define T_GRADIENT_COLOR T_("Gradient.Color")
#define T_GRADIENT_COLOR2 T_("Gradient.Color2")
#define T_GRADIENT_COLOR3 T_("Gradient.Color3")
#define T_GRADIENT_DIR T_("Gradient.Direction")
#define T_GRADIENT_OPACITY T_("Gradient.Opacity")
#define T_ALIGN T_("Alignment")
#define T_VALIGN T_("VerticalAlignment")
#define T_OPACITY T_("Opacity")
#define T_BKCOLOR T_("BkColor")
#define T_BKOPACITY T_("BkOpacity")
//#define T_VERTICAL                      T_("Vertical")
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

void obs_dwrite_text_source::UpdateFont()
{
	if (italic)
		style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC;
	else
		style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
}

void obs_dwrite_text_source::CalculateGradientAxis(float width, float height)
{
	if (width <= 0.0 || height <= 0.0)
		return;

	float angle = atan(height / width) * 180.0 / M_PI;

	if (gradient_dir <= angle || gradient_dir > 360.0 - angle) {
		float y = width / 2 * tan(gradient_dir * M_PI / 180.0);
		gradient_x = width;
		gradient_y = height / 2 - y;
		gradient2_x = 0;
		gradient2_y = height / 2 + y;

	} else if (gradient_dir <= 180.0 - angle && gradient_dir > angle) {
		float x = height / 2 * tan((90.0 - gradient_dir) * M_PI / 180.0);
		gradient_x = width / 2 + x;
		gradient_y = 0;
		gradient2_x = width / 2 - x;
		gradient2_y = height;
	} else if (gradient_dir <= 180.0 + angle && gradient_dir > 180.0 - angle) {
		float y = width / 2 * tan(gradient_dir * M_PI / 180.0);
		gradient_x = 0;
		gradient_y = height / 2 + y;
		gradient2_x = width;
		gradient2_y = height / 2 - y;
	} else {
		float x = height / 2 * tan((270.0 - gradient_dir) * M_PI / 180.0);
		gradient_x = width / 2 - x;
		gradient_y = height;
		gradient2_x = width / 2 + x;
		gradient2_y = 0;
	}
}

HRESULT obs_dwrite_text_source::InitializeDirectWrite()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, pD2DFactory.GetAddressOf());
	if (FAILED(hr))
		return hr;

	ComPtr<ID3D11Device> device((ID3D11Device *)gs_get_device_obj());
	if (device == nullptr)
		return E_FAIL;

	ComPtr<IDXGIDevice> dxgi;
	device.As(&dxgi);

	ComPtr<ID2D1Device> d2dDevice;
	hr = pD2DFactory->CreateDevice(dxgi.Get(), d2dDevice.GetAddressOf());
	if (FAILED(hr))
		return hr;

	d2dDevice.As(&pD2DDevice);

	hr = pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
					     pD2DDeviceContext.GetAddressOf());
	if (FAILED(hr))
		return hr;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory4),
				 reinterpret_cast<IUnknown **>(pDWriteFactory.GetAddressOf()));

	if (FAILED(hr))
		return hr;

	return hr;
}

void obs_dwrite_text_source::ReleaseResource() {}

void obs_dwrite_text_source::UpdateBrush(ComPtr<ID2D1DeviceContext4> pD2DDeviceContext,
					 ID2D1Brush **ppOutlineBrush, ID2D1Brush **ppFillBrush, float width,
					 float height)
{
	HRESULT hr;

	if ((uint32_t)gradient_count > 0) {
		CalculateGradientAxis(width, height);

		ComPtr<ID2D1GradientStopCollection> pGradientStops;

		float level = 1.0 / ((uint32_t)gradient_count - 1);

		D2D1_GRADIENT_STOP gradientStops[4];
		gradientStops[0].color = D2D1::ColorF(color, opacity / 100.0f);
		gradientStops[0].position = 0.0f;
		gradientStops[1].color = D2D1::ColorF(color2, opacity2 / 100.0f);
		gradientStops[1].position = gradientStops[0].position + level;
		gradientStops[2].color = D2D1::ColorF(color3, opacity2 / 100.0f);
		gradientStops[2].position = gradientStops[1].position + level;
		gradientStops[3].color = D2D1::ColorF(color4, opacity2 / 100.0f);
		gradientStops[3].position = 1.0f;

		hr = pD2DDeviceContext->CreateGradientStopCollection(gradientStops, (uint32_t)gradient_count,
								     D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_MIRROR,
								     pGradientStops.GetAddressOf());

		hr = pD2DDeviceContext->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(D2D1::Point2F(gradient_x, gradient_y),
							    D2D1::Point2F(gradient2_x, gradient2_y)),
			pGradientStops.Get(), (ID2D1LinearGradientBrush **)ppFillBrush);

	} else {
		hr = pD2DDeviceContext->CreateSolidColorBrush(D2D1::ColorF(color, opacity / 100.0f),
							      (ID2D1SolidColorBrush **)ppFillBrush);
	}

	if (use_outline) {
		hr = pD2DDeviceContext->CreateSolidColorBrush(D2D1::ColorF(outline_color,
									   outline_opacity / 100.0f),
							      (ID2D1SolidColorBrush **)ppOutlineBrush);
	} else {
		if (*ppOutlineBrush) {
			(*ppOutlineBrush)->Release();
			*ppOutlineBrush = nullptr;
		}
	}
}

void obs_dwrite_text_source::RenderText()
{
	if (pTextRenderer)
		pTextRenderer.Reset();

	UINT32 text_length = (UINT32)wcslen(text.c_str());
	HRESULT hr = S_OK;

	float layout_cx = (use_extents && extents_cx != -1) ? extents_cx : INFINITY;
	float layout_cy = (use_extents && extents_cy != -1) ? extents_cy : INFINITY;
	float text_cx = 0.0f;
	float text_cy = 0.0f;

	SIZE size = {0, 0};
	UINT32 lines = 1;

	if (pDWriteFactory) {

		WCHAR localeName[LOCALE_NAME_MAX_LENGTH] = {0};
		GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);

		hr = pDWriteFactory->CreateTextFormat(face.c_str(), NULL, weight, style, stretch,
						      (float)face_size / 96.0f * 72.0f, localeName,
						      &pTextFormat);
	}

	if (SUCCEEDED(hr)) {
		pTextFormat->SetTextAlignment(align);
		pTextFormat->SetParagraphAlignment(valign);
		pTextFormat->SetWordWrapping(wrap);

		/*if (vertical)
		{
		pTextFormat->SetReadingDirection(DWRITE_READING_DIRECTION_TOP_TO_BOTTOM);
		pTextFormat->SetFlowDirection(DWRITE_FLOW_DIRECTION_RIGHT_TO_LEFT);
		}*/

		hr = pDWriteFactory->CreateTextLayout(text.c_str(), text_length, pTextFormat.Get(), layout_cx,
						      layout_cy, pTextLayout.GetAddressOf());
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
			pDWriteFactory->CreateEllipsisTrimmingSign(pTextFormat.Get(),
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

		clamp(layout_cx, MIN_SIZE_CX, MAX_SIZE_CX);
		clamp(layout_cy, MIN_SIZE_CY, MAX_SIZE_CY);

		size.cx = layout_cx;
		size.cy = layout_cy;
	}

	if (!pTarget || !tex || (LONG)cx != size.cx || (LONG)cy != size.cy) {
		obs_enter_graphics();
		if (tex)
			gs_texture_destroy(tex);

		tex = gs_texture_create(size.cx, size.cy, GS_BGRA_UNORM, 1, nullptr, GS_RENDER_TARGET);

		ComPtr<ID3D11Texture2D> native_tex((ID3D11Texture2D *)gs_texture_get_obj(tex));
		ComPtr<IDXGISurface> native_surface;
		native_tex.As(&native_surface);

		D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), 96, 96);

		hr = pD2DDeviceContext->CreateBitmapFromDxgiSurface(native_surface.Get(), &bitmapProperties,
								    pTarget.ReleaseAndGetAddressOf());

		if (FAILED(hr) || pTarget == nullptr)
			return;

		pD2DDeviceContext->SetTarget(pTarget.Get());

		obs_leave_graphics();

		cx = (uint32_t)size.cx;
		cy = (uint32_t)size.cy;
	}

	if (SUCCEEDED(hr)) {
		UpdateBrush(pD2DDeviceContext, pOutlineBrush.GetAddressOf(), pFillBrush.GetAddressOf(), text_cx,
			    text_cy / lines);

		pTextRenderer = new obs_text_renderer(pDWriteFactory.Get(), pD2DFactory.Get(),
						      pD2DDeviceContext.Get(), pOutlineBrush.Get(),
						      pFillBrush.Get(), outline_size, color_fonts);

		pD2DDeviceContext->BeginDraw();
		pD2DDeviceContext->SetTextAntialiasMode(antialias ? D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE
								  : D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
		pD2DDeviceContext->SetTransform(D2D1::IdentityMatrix());
		pD2DDeviceContext->Clear(D2D1::ColorF(bk_color, bk_opacity / 100.0f));
		pTextLayout->Draw(NULL, pTextRenderer.Get(), 0, 0);
		hr = pD2DDeviceContext->EndDraw();
	}
}

std::string obs_dwrite_text_source::ProcessHtml(GumboNode *node, int position)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
	if (node->type == GUMBO_NODE_TEXT) {
		return std::string(node->v.text.original_text.data, node->v.text.original_text.length);
	} else if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag != GUMBO_TAG_SCRIPT &&
		   node->v.element.tag != GUMBO_TAG_STYLE) {
		std::string contents = "";
		GumboVector *children = &node->v.element.children;
		for (unsigned int i = 0; i < children->length; ++i) {
			dwrite_run run = {};

			auto child = ((GumboNode *)children->data[i]);
			auto content_length = utf8_conv.from_bytes(contents).length();

			run.start = position + content_length;

			if (node->v.element.tag == GUMBO_TAG_B || node->v.element.tag == GUMBO_TAG_STRONG) {
				run.format = format_flags::bold;
			}

			if (node->v.element.tag == GUMBO_TAG_I || node->v.element.tag == GUMBO_TAG_EM) {
				run.format = format_flags::italic;
			}

			if (node->v.element.tag == GUMBO_TAG_U || node->v.element.tag == GUMBO_TAG_INS) {
				run.format = format_flags::underline;
			}

			for (size_t j = 0; j < node->v.element.attributes.length; j++) {
				auto attribute = (GumboAttribute *)node->v.element.attributes.data[j];

				try {
					if (strcmpi(attribute->name, "size") == 0 &&
					    strlen(attribute->value) > 0) {
						run.size = std::stof(attribute->value) / 96.0f * 72.0f;
					}
				} catch (...) {
					// icky but stof is bad.
				}

				// TODO: parsing colours NotLikeThis
				//if (strcmpi(attribute->name, "colour")) {
				//	run.size = std::stof(attribute->value);
				//}
			}

			std::string text = ProcessHtml(child, position + content_length);
			if (node->v.element.tag == GUMBO_TAG_BR || node->v.element.tag == GUMBO_TAG_P ||
			    node->v.element.tag == GUMBO_TAG_H1 || node->v.element.tag == GUMBO_TAG_H2 ||
			    node->v.element.tag == GUMBO_TAG_H3 || node->v.element.tag == GUMBO_TAG_H4 ||
			    node->v.element.tag == GUMBO_TAG_H5 || node->v.element.tag == GUMBO_TAG_H6 ||
			    node->v.element.tag == GUMBO_TAG_DIV) {
				text.append("\r\n");
			}

			run.length = utf8_conv.from_bytes(text).length();

			//if (run.format != format_flags::none)
			runs.push_back(run);
			contents.append(text);
		}
		return contents;
	} else {
		return "";
	}
}

void obs_dwrite_text_source::TransformText()
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

	if (use_html) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
		std::string utf8 = utf8_conv.to_bytes(text);
		std::string newText;

		GumboOutput *parsed =
			gumbo_parse_with_options(&kGumboDefaultOptions, utf8.c_str(), utf8.size());
		newText = ProcessHtml(parsed->root, 0);
		gumbo_destroy_output(&kGumboDefaultOptions, parsed);

		text = utf8_conv.from_bytes(newText);
	}
}

const char *obs_dwrite_text_source::GetMainString(const char *str)
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

void obs_dwrite_text_source::LoadFileText()
{
	BPtr<char> file_text = os_quick_read_utf8_file(file.c_str());
	text = to_wide(GetMainString(file_text));
}

#define obs_data_get_uint32 (uint32_t) obs_data_get_int

inline void obs_dwrite_text_source::Update(obs_data_t *s)
{
	const char *new_text = obs_data_get_string(s, S_TEXT);
	obs_data_t *font_obj = obs_data_get_obj(s, S_FONT);
	const char *align_str = obs_data_get_string(s, S_ALIGN);
	const char *valign_str = obs_data_get_string(s, S_VALIGN);
	const char *wrap_str = obs_data_get_string(s, S_WRAP_MODE);
	uint32_t new_color = obs_data_get_uint32(s, S_COLOR);
	uint32_t new_opacity = obs_data_get_uint32(s, S_OPACITY);
	const char *gradient_str = obs_data_get_string(s, S_GRADIENT);
	uint32_t new_color2 = obs_data_get_uint32(s, S_GRADIENT_COLOR);
	uint32_t new_color3 = obs_data_get_uint32(s, S_GRADIENT_COLOR2);
	uint32_t new_color4 = obs_data_get_uint32(s, S_GRADIENT_COLOR3);
	uint32_t new_opacity2 = obs_data_get_uint32(s, S_GRADIENT_OPACITY);
	float new_grad_dir = (float)obs_data_get_double(s, S_GRADIENT_DIR);
	//bool new_vertical = obs_data_get_bool(s, S_VERTICAL);
	bool new_outline = obs_data_get_bool(s, S_OUTLINE);
	uint32_t new_o_color = obs_data_get_uint32(s, S_OUTLINE_COLOR);
	uint32_t new_o_opacity = obs_data_get_uint32(s, S_OUTLINE_OPACITY);
	uint32_t new_o_size = obs_data_get_uint32(s, S_OUTLINE_SIZE);
	bool new_use_file = obs_data_get_bool(s, S_USE_FILE);
	const char *new_file = obs_data_get_string(s, S_FILE);
	bool new_chat_mode = obs_data_get_bool(s, S_CHATLOG_MODE);
	int new_chat_lines = (int)obs_data_get_int(s, S_CHATLOG_LINES);
	bool new_extents = obs_data_get_bool(s, S_EXTENTS);
	int32_t n_extents_cx = obs_data_get_int(s, S_EXTENTS_CX);
	int32_t n_extents_cy = obs_data_get_int(s, S_EXTENTS_CY);

	const char *font_face = obs_data_get_string(font_obj, "face");
	int font_size = (int)obs_data_get_int(font_obj, "size");
	int64_t font_flags = obs_data_get_int(font_obj, "flags");
	//bool new_bold = (font_flags & OBS_FONT_BOLD) != 0;
	bool new_italic = (font_flags & OBS_FONT_ITALIC) != 0;
	bool new_underline = (font_flags & OBS_FONT_UNDERLINE) != 0;
	bool new_strikeout = (font_flags & OBS_FONT_STRIKEOUT) != 0;

	bool new_color_fonts = obs_data_get_bool(s, S_COLOR_FONTS);
	bool new_antialias = obs_data_get_bool(s, S_ANTIALIASING);

	int new_text_transform = (int)obs_data_get_int(s, S_TRANSFORM);
	int new_text_trimming = (int)obs_data_get_int(s, S_TRIMMING);

	int new_font_weight = (int)obs_data_get_int(s, S_FONT_WEIGHT);
	int new_font_stretch = (int)obs_data_get_int(s, S_FONT_STRETCH);

	uint32_t new_bk_color = obs_data_get_uint32(s, S_BKCOLOR);
	uint32_t new_bk_opacity = obs_data_get_uint32(s, S_BKOPACITY);

	bool new_html = obs_data_get_bool(s, S_HTML);

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

		UpdateFont();
	}

	/* ----------------------------- */

	new_color = rgb_to_bgr(new_color);
	new_color2 = rgb_to_bgr(new_color2);
	new_color3 = rgb_to_bgr(new_color3);
	new_color4 = rgb_to_bgr(new_color4);
	new_o_color = rgb_to_bgr(new_o_color);
	new_bk_color = rgb_to_bgr(new_bk_color);

	color = new_color;
	opacity = new_opacity;
	color2 = new_color2;
	color3 = new_color3;
	color4 = new_color4;
	opacity2 = new_opacity2;
	gradient_dir = new_grad_dir;
	//vertical = new_vertical;
	color_fonts = new_color_fonts;
	antialias = new_antialias;
	text_transform = new_text_transform;
	text_trimming = new_text_trimming;
	use_html = new_html;

	gradient_mode new_count = gradient_mode::none;
	if (strcmp(gradient_str, S_GRADIENT_NONE) == 0)
		new_count = gradient_mode::none;
	else if (strcmp(gradient_str, S_GRADIENT_TWO) == 0)
		new_count = gradient_mode::two_colour;
	else if (strcmp(gradient_str, S_GRADIENT_THREE) == 0)
		new_count = gradient_mode::three_colour;
	else
		new_count = gradient_mode::four_colour;

	gradient_count = new_count;

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
		LoadFileText();

	} else {
		text = to_wide(GetMainString(new_text));
	}

	TransformText();

	use_outline = new_outline;
	outline_color = new_o_color;
	outline_opacity = new_o_opacity;
	outline_size = roundf(float(new_o_size));

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

	RenderText();
	update_time_elapsed = 0.0f;

	/* ----------------------------- */

	obs_data_release(font_obj);
}

inline void obs_dwrite_text_source::Tick(float seconds)
{
	if (!read_from_file)
		return;

	update_time_elapsed += seconds;

	if (update_time_elapsed >= 1.0f) {
		time_t t = get_modified_timestamp(file.c_str());
		update_time_elapsed = 0.0f;

		if (file_timestamp != t) {
			LoadFileText();
			TransformText();
			RenderText();
			file_timestamp = t;
		}
	}
}

inline void obs_dwrite_text_source::Render(gs_effect_t *effect)
{
	if (!tex)
		return;

	gs_effect_set_texture(gs_effect_get_param_by_name(effect, "image"), tex);
	gs_draw_sprite(tex, 0, cx, cy);
}

/* ------------------------------------------------------------------------- */

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-text", "en-US")
MODULE_EXPORT const char *obs_module_description(void)
{
	return "Windows DirectWrite/Direct2 text source";
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

static bool chatlog_mode_changed(obs_properties_t *props, obs_property_t *p, obs_data_t *s)
{
	bool chatlog_mode = obs_data_get_bool(s, S_CHATLOG_MODE);

	set_vis(chatlog_mode, S_CHATLOG_LINES, true);
	return true;
}

static bool gradient_changed(obs_properties_t *props, obs_property_t *p, obs_data_t *s)
{
	const char *gradient_str = obs_data_get_string(s, S_GRADIENT);
	gradient_mode mode = gradient_mode::none;

	if (strcmp(gradient_str, S_GRADIENT_NONE) == 0)
		mode = gradient_mode::none;
	else if (strcmp(gradient_str, S_GRADIENT_TWO) == 0)
		mode = gradient_mode::two_colour;
	else if (strcmp(gradient_str, S_GRADIENT_THREE) == 0)
		mode = gradient_mode::three_colour;
	else
		mode = gradient_mode::four_colour;

	bool gradient_color = (mode > gradient_mode::none);
	bool gradient_color2 = (mode > gradient_mode::two_colour);
	bool gradient_color3 = (mode > gradient_mode::three_colour);

	set_vis(gradient_color, S_GRADIENT_COLOR, true);
	set_vis(gradient_color2, S_GRADIENT_COLOR2, true);
	set_vis(gradient_color3, S_GRADIENT_COLOR3, true);
	set_vis(gradient_color, S_GRADIENT_OPACITY, true);
	set_vis(gradient_color, S_GRADIENT_DIR, true);

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

	obs_properties_add_font(props, S_FONT, T_FONT);

	p = obs_properties_add_bool(props, S_USE_FILE, T_USE_FILE);
	obs_property_set_modified_callback(p, use_file_changed);

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

	obs_properties_add_text(props, S_TEXT, T_TEXT, OBS_TEXT_MULTILINE);
	obs_properties_add_path(props, S_FILE, T_FILE, OBS_PATH_FILE, filter.c_str(), path.c_str());

	obs_properties_add_bool(props, S_HTML, T_HTML);

	//obs_properties_add_bool(props, S_VERTICAL, T_VERTICAL);
	obs_properties_add_color(props, S_COLOR, T_COLOR);
	obs_properties_add_int_slider(props, S_OPACITY, T_OPACITY, 0, 100, 1);

	p = obs_properties_add_list(props, S_FONT_WEIGHT, T_FONT_WEIGHT, OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_INT);
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

	p = obs_properties_add_list(props, S_FONT_STRETCH, T_FONT_STRETCH, OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, T_FONT_STRETCH_ULTRA_CONDENSED, S_FONT_STRETCH_ULTRA_CONDENSED);
	obs_property_list_add_int(p, T_FONT_STRETCH_EXTRA_CONDENSED, S_FONT_STRETCH_EXTRA_CONDENSED);
	obs_property_list_add_int(p, T_FONT_STRETCH_CONDENSED, S_FONT_STRETCH_CONDENSED);
	obs_property_list_add_int(p, T_FONT_STRETCH_SEMI_CONDENSED, S_FONT_STRETCH_SEMI_CONDENSED);
	obs_property_list_add_int(p, T_FONT_STRETCH_NORMAL, S_FONT_STRETCH_NORMAL);
	obs_property_list_add_int(p, T_FONT_STRETCH_SEMI_EXPANDED, S_FONT_STRETCH_SEMI_EXPANDED);
	obs_property_list_add_int(p, T_FONT_STRETCH_EXPANDED, S_FONT_STRETCH_EXPANDED);
	obs_property_list_add_int(p, T_FONT_STRETCH_EXTRA_EXPANDED, S_FONT_STRETCH_EXTRA_EXPANDED);
	obs_property_list_add_int(p, T_FONT_STRETCH_ULTRA_EXPANDED, S_FONT_STRETCH_ULTRA_EXPANDED);

	/*p = obs_properties_add_bool(props, S_GRADIENT, T_GRADIENT);
	obs_property_set_modified_callback(p, gradient_changed);*/

	p = obs_properties_add_list(props, S_GRADIENT, T_GRADIENT, OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);

	obs_property_list_add_string(p, T_GRADIENT_NONE, S_GRADIENT_NONE);
	obs_property_list_add_string(p, T_GRADIENT_TWO, S_GRADIENT_TWO);
	obs_property_list_add_string(p, T_GRADIENT_THREE, S_GRADIENT_THREE);
	obs_property_list_add_string(p, T_GRADIENT_FOUR, S_GRADIENT_FOUR);

	obs_property_set_modified_callback(p, gradient_changed);

	obs_properties_add_color(props, S_GRADIENT_COLOR, T_GRADIENT_COLOR);
	obs_properties_add_color(props, S_GRADIENT_COLOR2, T_GRADIENT_COLOR2);
	obs_properties_add_color(props, S_GRADIENT_COLOR3, T_GRADIENT_COLOR3);
	obs_properties_add_int_slider(props, S_GRADIENT_OPACITY, T_GRADIENT_OPACITY, 0, 100, 1);
	obs_properties_add_float_slider(props, S_GRADIENT_DIR, T_GRADIENT_DIR, 0, 360, 0.1);

	obs_properties_add_color(props, S_BKCOLOR, T_BKCOLOR);
	obs_properties_add_int_slider(props, S_BKOPACITY, T_BKOPACITY, 0, 100, 1);

	p = obs_properties_add_list(props, S_ALIGN, T_ALIGN, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, T_ALIGN_LEFT, S_ALIGN_LEFT);
	obs_property_list_add_string(p, T_ALIGN_CENTER, S_ALIGN_CENTER);
	obs_property_list_add_string(p, T_ALIGN_RIGHT, S_ALIGN_RIGHT);
	obs_property_list_add_string(p, T_ALIGN_JUSTIFIED, S_ALIGN_JUSTIFIED);

	p = obs_properties_add_list(props, S_VALIGN, T_VALIGN, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, T_VALIGN_TOP, S_VALIGN_TOP);
	obs_property_list_add_string(p, T_VALIGN_CENTER, S_VALIGN_CENTER);
	obs_property_list_add_string(p, T_VALIGN_BOTTOM, S_VALIGN_BOTTOM);

	p = obs_properties_add_list(props, S_TRANSFORM, T_TRANSFORM, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, T_TRANSFORM_NONE, S_TRANSFORM_NONE);
	obs_property_list_add_int(p, T_TRANSFORM_UPPERCASE, S_TRANSFORM_UPPERCASE);
	obs_property_list_add_int(p, T_TRANSFORM_LOWERCASE, S_TRANSFORM_LOWERCASE);
	obs_property_list_add_int(p, T_TRANSFORM_STARTCASE, S_TRANSFORM_STARTCASE);

	p = obs_properties_add_list(props, S_WRAP_MODE, T_WRAP_MODE, OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, T_WRAP_MODE_NONE, S_WRAP_MODE_NONE);
	obs_property_list_add_string(p, T_WRAP_MODE_WRAP, S_WRAP_MODE_WRAP);
	obs_property_list_add_string(p, T_WRAP_MODE_WRAP_CHARACTER, S_WRAP_MODE_WRAP_CHARACTER);
	obs_property_list_add_string(p, T_WRAP_MODE_WRAP_WHOLE_WORDS, S_WRAP_MODE_WRAP_WHOLE_WORDS);

	p = obs_properties_add_list(props, S_TRIMMING, T_TRIMMING, OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(p, T_TRIMMING_NONE, S_TRIMMING_NONE);
	obs_property_list_add_int(p, T_TRIMMING_CHARACTER_ELLIPSIS, S_TRIMMING_CHARACTER_ELLIPSIS);
	obs_property_list_add_int(p, T_TRIMMING_WORD_ELLIPSIS, S_TRIMMING_WORD_ELLIPSIS);

	p = obs_properties_add_bool(props, S_OUTLINE, T_OUTLINE);
	obs_property_set_modified_callback(p, outline_changed);

	obs_properties_add_int(props, S_OUTLINE_SIZE, T_OUTLINE_SIZE, 1, 20, 1);
	obs_properties_add_color(props, S_OUTLINE_COLOR, T_OUTLINE_COLOR);
	obs_properties_add_int_slider(props, S_OUTLINE_OPACITY, T_OUTLINE_OPACITY, 0, 100, 1);

	p = obs_properties_add_bool(props, S_CHATLOG_MODE, T_CHATLOG_MODE);
	obs_property_set_modified_callback(p, chatlog_mode_changed);

	obs_properties_add_int(props, S_CHATLOG_LINES, T_CHATLOG_LINES, 1, 1000, 1);

	p = obs_properties_add_bool(props, S_ANTIALIASING, T_ANTIALIASING);
	p = obs_properties_add_bool(props, S_COLOR_FONTS, T_COLOR_FONTS);

	p = obs_properties_add_bool(props, S_EXTENTS, T_EXTENTS);
	obs_property_set_modified_callback(p, extents_modified);

	obs_properties_add_int(props, S_EXTENTS_CX, T_EXTENTS_CX, -1, 8000, 1);
	obs_properties_add_int(props, S_EXTENTS_CY, T_EXTENTS_CY, -1, 8000, 1);

	return props;
}

bool obs_module_load(void)
{
	obs_source_info si = {};
	si.id = "text_directwrite";
	si.type = OBS_SOURCE_TYPE_INPUT;
	si.output_flags = OBS_SOURCE_VIDEO;
	si.get_properties = get_properties;
	si.icon_type = OBS_ICON_TYPE_TEXT;

	si.get_name = [](void *) { return obs_module_text("TextDirectWrite"); };
	si.create = [](obs_data_t *settings, obs_source_t *source) {
		return (void *)new obs_dwrite_text_source(source, settings);
	};
	si.destroy = [](void *data) { delete reinterpret_cast<obs_dwrite_text_source *>(data); };
	si.get_width = [](void *data) { return reinterpret_cast<obs_dwrite_text_source *>(data)->cx; };
	si.get_height = [](void *data) { return reinterpret_cast<obs_dwrite_text_source *>(data)->cy; };
	si.get_defaults = [](obs_data_t *settings) {
		obs_data_t *font_obj = obs_data_create();
		obs_data_set_default_string(font_obj, "face", "Arial");
		obs_data_set_default_int(font_obj, "size", 256);

		obs_data_set_default_obj(settings, S_FONT, font_obj);
		obs_data_set_default_string(settings, S_ALIGN, S_ALIGN_LEFT);
		obs_data_set_default_string(settings, S_VALIGN, S_VALIGN_TOP);
		obs_data_set_default_string(settings, S_WRAP_MODE, S_WRAP_MODE_WRAP);
		obs_data_set_default_int(settings, S_FONT_WEIGHT, S_FONT_WEIGHT_400);
		obs_data_set_default_int(settings, S_FONT_STRETCH, S_FONT_STRETCH_NORMAL);
		obs_data_set_default_int(settings, S_COLOR, 0xFFFFFF);
		obs_data_set_default_int(settings, S_OPACITY, 100);
		obs_data_set_default_int(settings, S_GRADIENT_COLOR, 0xFFFFFF);
		obs_data_set_default_int(settings, S_GRADIENT_COLOR2, 0xFFFFFF);
		obs_data_set_default_int(settings, S_GRADIENT_COLOR3, 0xFFFFFF);
		obs_data_set_default_int(settings, S_GRADIENT_OPACITY, 100);
		obs_data_set_default_double(settings, S_GRADIENT_DIR, 90.0);
		obs_data_set_default_int(settings, S_BKCOLOR, 0x000000);
		obs_data_set_default_int(settings, S_BKOPACITY, 0);
		obs_data_set_default_int(settings, S_OUTLINE_SIZE, 2);
		obs_data_set_default_int(settings, S_OUTLINE_COLOR, 0xFFFFFF);
		obs_data_set_default_int(settings, S_OUTLINE_OPACITY, 100);
		obs_data_set_default_int(settings, S_CHATLOG_LINES, 6);
		obs_data_set_default_int(settings, S_EXTENTS_CX, 100);
		obs_data_set_default_int(settings, S_EXTENTS_CY, 100);
		obs_data_set_default_bool(settings, S_COLOR_FONTS, true);
		obs_data_set_default_bool(settings, S_ANTIALIASING, true);
		obs_data_set_default_bool(settings, S_HTML, false);

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
