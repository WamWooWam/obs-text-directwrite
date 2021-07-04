
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

#define S_VALIGN_TOP "top"
#define S_VALIGN_CENTER S_ALIGN_CENTER
#define S_VALIGN_BOTTOM "bottom"

#define S_WRAP_MODE "wrap_mode"
#define S_WRAP_MODE_NONE "none"
#define S_WRAP_MODE_WRAP "wrap"
#define S_WRAP_MODE_WRAP_WHOLE_WORDS "wrap_whole_words"
#define S_WRAP_MODE_WRAP_CHARACTER "wrap_character"

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

#define T_VALIGN_TOP T_("VerticalAlignment.Top")
#define T_VALIGN_CENTER T_ALIGN_CENTER
#define T_VALIGN_BOTTOM T_("VerticalAlignment.Bottom")

#define T_WRAP_MODE T_("WordWrap")
#define T_WRAP_MODE_NONE T_("WordWrap.None")
#define T_WRAP_MODE_WRAP T_("WordWrap.Wrap")
#define T_WRAP_MODE_WRAP_WHOLE_WORDS T_("WordWrap.WrapWholeWords")
#define T_WRAP_MODE_WRAP_CHARACTER T_("WordWrap.WrapCharacter")

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
	if (bold)
		weight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_BOLD;
	else
		weight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_REGULAR;

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
		float x =
			height / 2 * tan((90.0 - gradient_dir) * M_PI / 180.0);
		gradient_x = width / 2 + x;
		gradient_y = 0;
		gradient2_x = width / 2 - x;
		gradient2_y = height;
	} else if (gradient_dir <= 180.0 + angle &&
		   gradient_dir > 180.0 - angle) {
		float y = width / 2 * tan(gradient_dir * M_PI / 180.0);
		gradient_x = 0;
		gradient_y = height / 2 + y;
		gradient2_x = width;
		gradient2_y = height / 2 - y;
	} else {
		float x =
			height / 2 * tan((270.0 - gradient_dir) * M_PI / 180.0);
		gradient_x = width / 2 - x;
		gradient_y = height;
		gradient2_x = width / 2 + x;
		gradient2_y = 0;
	}
}

HRESULT obs_dwrite_text_source::InitializeDirectWrite()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
			       pD2DFactory.GetAddressOf());
	if (FAILED(hr))
		return hr;

	ID3D11Device *device = (ID3D11Device *)gs_get_device_obj();
	if (device == nullptr)
		return E_FAIL;

	IDXGIDevice *dxgi = nullptr;
	hr = device->QueryInterface(&dxgi);

	if (FAILED(hr))
		return hr;

	ID2D1Device *d2dDevice = nullptr;
	hr = pD2DFactory->CreateDevice(dxgi, &d2dDevice);
	if (FAILED(hr))
		return hr;

	hr = d2dDevice->QueryInterface(pD2DDevice.GetAddressOf());
	if (FAILED(hr))
		return hr;

	hr = pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
					     &pD2DDeviceContext);
	if (FAILED(hr))
		return hr;

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory4),
		reinterpret_cast<IUnknown **>(pDWriteFactory.GetAddressOf()));
	if (FAILED(hr))
		return hr;

	return hr;
}

void obs_dwrite_text_source::ReleaseResource()
{
	SafeRelease(&pTextRenderer);
}

void obs_dwrite_text_source::UpdateBrush(ComPtr<ID2D1DeviceContext4> pD2DDeviceContext,
			     ID2D1Brush **ppOutlineBrush,
			     ID2D1Brush **ppFillBrush, float width,
			     float height)
{
	HRESULT hr;

	if ((uint32_t)gradient_count > 0) {
		CalculateGradientAxis(width, height);

		ID2D1GradientStopCollection *pGradientStops = NULL;

		float level = 1.0 / ((uint32_t)gradient_count - 1);

		D2D1_GRADIENT_STOP gradientStops[4];
		gradientStops[0].color = D2D1::ColorF(color, opacity / 100.0);
		gradientStops[0].position = 0.0f;
		gradientStops[1].color = D2D1::ColorF(color2, opacity2 / 100.0);
		gradientStops[1].position = gradientStops[0].position + level;
		gradientStops[2].color = D2D1::ColorF(color3, opacity2 / 100.0);
		gradientStops[2].position = gradientStops[1].position + level;
		gradientStops[3].color = D2D1::ColorF(color4, opacity2 / 100.0);
		gradientStops[3].position = 1.0f;

		hr = pD2DDeviceContext->CreateGradientStopCollection(
			gradientStops, (uint32_t)gradient_count, D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_MIRROR, &pGradientStops);

		hr = pD2DDeviceContext->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(gradient_x, gradient_y),
				D2D1::Point2F(gradient2_x, gradient2_y)),
			pGradientStops,
			(ID2D1LinearGradientBrush **)ppFillBrush);

	} else {
		hr = pD2DDeviceContext->CreateSolidColorBrush(
			D2D1::ColorF(color, opacity / 100.0),
			(ID2D1SolidColorBrush **)ppFillBrush);
	}

	if (use_outline) {
		hr = pD2DDeviceContext->CreateSolidColorBrush(
			D2D1::ColorF(outline_color, outline_opacity / 100.0),
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
	UINT32 TextLength = (UINT32)wcslen(text.c_str());
	HRESULT hr = S_OK;

	float layout_cx = (use_extents) ? extents_cx : INFINITY;
	float layout_cy = (use_extents) ? extents_cy : INFINITY;
	float text_cx = 0.0;
	float text_cy = 0.0;

	SIZE size = {0, 0};
	UINT32 lines = 1;

	if (pDWriteFactory) {

		WCHAR localeName[LOCALE_NAME_MAX_LENGTH] = {0};
		GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);

		hr = pDWriteFactory->CreateTextFormat(
			face.c_str(), NULL, weight, style, stretch,
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

		hr = pDWriteFactory->CreateTextLayout(
			text.c_str(), TextLength, pTextFormat.Get(), layout_cx,
			layout_cy, pTextLayout.GetAddressOf());
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

		if (!use_extents) {
			layout_cx = text_cx;
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

		tex = gs_texture_create(size.cx, size.cy, GS_BGRA_UNORM, 1,
					nullptr, GS_RENDER_TARGET);

		ID3D11Texture2D *native_tex =
			(ID3D11Texture2D *)gs_texture_get_obj(tex);
		IDXGISurface *native_surface = nullptr;
		hr = native_tex->QueryInterface(&native_surface);

		if (FAILED(hr) || native_surface == nullptr)
			goto cleanup;

		D2D1_BITMAP_PROPERTIES1 bitmapProperties =
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET |
					D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
						  D2D1_ALPHA_MODE_PREMULTIPLIED),
				96, 96);

		hr = pD2DDeviceContext->CreateBitmapFromDxgiSurface(
			native_surface, &bitmapProperties,
			pTarget.ReleaseAndGetAddressOf());
		if (FAILED(hr) || pTarget == nullptr)
			goto cleanup;

		pD2DDeviceContext->SetTarget(pTarget.Get());

		obs_leave_graphics();

		cx = (uint32_t)size.cx;
		cy = (uint32_t)size.cy;
	}

	if (SUCCEEDED(hr)) {
		DWRITE_TEXT_RANGE text_range = {0, TextLength};
		pTextLayout->SetUnderline(underline, text_range);
		pTextLayout->SetStrikethrough(strikeout, text_range);
		pTextLayout->SetMaxWidth(layout_cx);
		pTextLayout->SetMaxHeight(layout_cy);
	}

	if (SUCCEEDED(hr)) {
		UpdateBrush(pD2DDeviceContext, pOutlineBrush.GetAddressOf(),
			    pFillBrush.GetAddressOf(), text_cx,
			    text_cy / lines);

		pTextRenderer = new (std::nothrow) obs_text_renderer(
			pDWriteFactory.Get(), pD2DFactory.Get(),
			pD2DDeviceContext.Get(), pOutlineBrush.Get(),
			pFillBrush.Get(), outline_size);

		pD2DDeviceContext->BeginDraw();
		pD2DDeviceContext->SetTransform(D2D1::IdentityMatrix());
		pD2DDeviceContext->Clear(
			D2D1::ColorF(bk_color, bk_opacity / 100.0));
		pTextLayout->Draw(NULL, pTextRenderer, 0, 0);
		hr = pD2DDeviceContext->EndDraw();
	}

cleanup:
	SafeRelease(&pTextRenderer);
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
	uint32_t n_extents_cx = obs_data_get_uint32(s, S_EXTENTS_CX);
	uint32_t n_extents_cy = obs_data_get_uint32(s, S_EXTENTS_CY);

	const char *font_face = obs_data_get_string(font_obj, "face");
	int font_size = (int)obs_data_get_int(font_obj, "size");
	int64_t font_flags = obs_data_get_int(font_obj, "flags");
	bool new_bold = (font_flags & OBS_FONT_BOLD) != 0;
	bool new_italic = (font_flags & OBS_FONT_ITALIC) != 0;
	bool new_underline = (font_flags & OBS_FONT_UNDERLINE) != 0;
	bool new_strikeout = (font_flags & OBS_FONT_STRIKEOUT) != 0;

	uint32_t new_bk_color = obs_data_get_uint32(s, S_BKCOLOR);
	uint32_t new_bk_opacity = obs_data_get_uint32(s, S_BKOPACITY);

	/* ----------------------------- */

	std::wstring new_face = to_wide(font_face);

	if (new_face != face || face_size != font_size || new_bold != bold ||
	    new_italic != italic || new_underline != underline ||
	    new_strikeout != strikeout) {

		face = new_face;
		face_size = font_size;
		bold = new_bold;
		italic = new_italic;
		underline = new_underline;
		strikeout = new_strikeout;

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

	use_outline = new_outline;
	outline_color = new_o_color;
	outline_opacity = new_o_opacity;
	outline_size = roundf(float(new_o_size));

	if (strcmp(align_str, S_ALIGN_CENTER) == 0)
		align = DWRITE_TEXT_ALIGNMENT_CENTER;
	else if (strcmp(align_str, S_ALIGN_RIGHT) == 0)
		align = DWRITE_TEXT_ALIGNMENT_TRAILING;
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
			RenderText();
			file_timestamp = t;
		}
	}
}

inline void obs_dwrite_text_source::Render(gs_effect_t *effect)
{
	if (!tex)
		return;

	gs_effect_set_texture(gs_effect_get_param_by_name(effect, "image"),
			      tex);
	gs_draw_sprite(tex, 0, cx, cy);
}

/* ------------------------------------------------------------------------- */

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-text", "en-US")

#define set_vis(var, val, show)                           \
	do {                                              \
		p = obs_properties_get(props, val);       \
		obs_property_set_visible(p, var == show); \
	} while (false)

static bool use_file_changed(obs_properties_t *props, obs_property_t *p,
			     obs_data_t *s)
{
	bool use_file = obs_data_get_bool(s, S_USE_FILE);

	set_vis(use_file, S_TEXT, false);
	set_vis(use_file, S_FILE, true);
	return true;
}

static bool outline_changed(obs_properties_t *props, obs_property_t *p,
			    obs_data_t *s)
{
	bool outline = obs_data_get_bool(s, S_OUTLINE);

	set_vis(outline, S_OUTLINE_SIZE, true);
	set_vis(outline, S_OUTLINE_COLOR, true);
	set_vis(outline, S_OUTLINE_OPACITY, true);
	return true;
}

static bool chatlog_mode_changed(obs_properties_t *props, obs_property_t *p,
				 obs_data_t *s)
{
	bool chatlog_mode = obs_data_get_bool(s, S_CHATLOG_MODE);

	set_vis(chatlog_mode, S_CHATLOG_LINES, true);
	return true;
}

static bool gradient_changed(obs_properties_t *props, obs_property_t *p,
			     obs_data_t *s)
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

static bool extents_modified(obs_properties_t *props, obs_property_t *p,
			     obs_data_t *s)
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
	obs_properties_add_path(props, S_FILE, T_FILE, OBS_PATH_FILE,
				filter.c_str(), path.c_str());

	//obs_properties_add_bool(props, S_VERTICAL, T_VERTICAL);
	obs_properties_add_color(props, S_COLOR, T_COLOR);
	obs_properties_add_int_slider(props, S_OPACITY, T_OPACITY, 0, 100, 1);

	/*p = obs_properties_add_bool(props, S_GRADIENT, T_GRADIENT);
	obs_property_set_modified_callback(p, gradient_changed);*/

	p = obs_properties_add_list(props, S_GRADIENT, T_GRADIENT,
				    OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);

	obs_property_list_add_string(p, T_GRADIENT_NONE, S_GRADIENT_NONE);
	obs_property_list_add_string(p, T_GRADIENT_TWO, S_GRADIENT_TWO);
	obs_property_list_add_string(p, T_GRADIENT_THREE, S_GRADIENT_THREE);
	obs_property_list_add_string(p, T_GRADIENT_FOUR, S_GRADIENT_FOUR);

	obs_property_set_modified_callback(p, gradient_changed);

	obs_properties_add_color(props, S_GRADIENT_COLOR, T_GRADIENT_COLOR);
	obs_properties_add_color(props, S_GRADIENT_COLOR2, T_GRADIENT_COLOR2);
	obs_properties_add_color(props, S_GRADIENT_COLOR3, T_GRADIENT_COLOR3);
	obs_properties_add_int_slider(props, S_GRADIENT_OPACITY,
				      T_GRADIENT_OPACITY, 0, 100, 1);
	obs_properties_add_float_slider(props, S_GRADIENT_DIR, T_GRADIENT_DIR,
					0, 360, 0.1);

	obs_properties_add_color(props, S_BKCOLOR, T_BKCOLOR);
	obs_properties_add_int_slider(props, S_BKOPACITY, T_BKOPACITY, 0, 100,
				      1);

	p = obs_properties_add_list(props, S_ALIGN, T_ALIGN,
				    OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, T_ALIGN_LEFT, S_ALIGN_LEFT);
	obs_property_list_add_string(p, T_ALIGN_CENTER, S_ALIGN_CENTER);
	obs_property_list_add_string(p, T_ALIGN_RIGHT, S_ALIGN_RIGHT);

	p = obs_properties_add_list(props, S_VALIGN, T_VALIGN,
				    OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, T_VALIGN_TOP, S_VALIGN_TOP);
	obs_property_list_add_string(p, T_VALIGN_CENTER, S_VALIGN_CENTER);
	obs_property_list_add_string(p, T_VALIGN_BOTTOM, S_VALIGN_BOTTOM);

	p = obs_properties_add_list(props, S_WRAP_MODE, T_WRAP_MODE,
				    OBS_COMBO_TYPE_LIST,
				    OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(p, T_WRAP_MODE_NONE, S_WRAP_MODE_NONE);
	obs_property_list_add_string(p, T_WRAP_MODE_WRAP, S_WRAP_MODE_WRAP);
	obs_property_list_add_string(p, T_WRAP_MODE_WRAP_CHARACTER,
				     S_WRAP_MODE_WRAP_CHARACTER);
	obs_property_list_add_string(p, T_WRAP_MODE_WRAP_WHOLE_WORDS,
				     S_WRAP_MODE_WRAP_WHOLE_WORDS);

	p = obs_properties_add_bool(props, S_OUTLINE, T_OUTLINE);
	obs_property_set_modified_callback(p, outline_changed);

	obs_properties_add_int(props, S_OUTLINE_SIZE, T_OUTLINE_SIZE, 1, 20, 1);
	obs_properties_add_color(props, S_OUTLINE_COLOR, T_OUTLINE_COLOR);
	obs_properties_add_int_slider(props, S_OUTLINE_OPACITY,
				      T_OUTLINE_OPACITY, 0, 100, 1);

	p = obs_properties_add_bool(props, S_CHATLOG_MODE, T_CHATLOG_MODE);
	obs_property_set_modified_callback(p, chatlog_mode_changed);

	obs_properties_add_int(props, S_CHATLOG_LINES, T_CHATLOG_LINES, 1, 1000,
			       1);

	p = obs_properties_add_bool(props, S_EXTENTS, T_EXTENTS);
	obs_property_set_modified_callback(p, extents_modified);

	obs_properties_add_int(props, S_EXTENTS_CX, T_EXTENTS_CX, 32, 8000, 1);
	obs_properties_add_int(props, S_EXTENTS_CY, T_EXTENTS_CY, 32, 8000, 1);

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
	si.destroy = [](void *data) {
		delete reinterpret_cast<obs_dwrite_text_source *>(data);
	};
	si.get_width = [](void *data) {
		return reinterpret_cast<obs_dwrite_text_source *>(data)->cx;
	};
	si.get_height = [](void *data) {
		return reinterpret_cast<obs_dwrite_text_source *>(data)->cy;
	};
	si.get_defaults = [](obs_data_t *settings) {
		obs_data_t *font_obj = obs_data_create();
		obs_data_set_default_string(font_obj, "face", "Arial");
		obs_data_set_default_int(font_obj, "size", 256);

		obs_data_set_default_obj(settings, S_FONT, font_obj);
		obs_data_set_default_string(settings, S_ALIGN, S_ALIGN_LEFT);
		obs_data_set_default_string(settings, S_VALIGN, S_VALIGN_TOP);
		obs_data_set_default_string(settings, S_WRAP_MODE,
					    S_WRAP_MODE_WRAP);
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
