#include <dwrite.h>
#include <d2d1.h>
#include <d3d11.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wrl.h>

using namespace Microsoft::WRL;

class OBSTextRenderer : public IDWriteTextRenderer {
public:
	OBSTextRenderer(IDWriteFactory4 *pDWriteFactory_, ID2D1Factory *pD2DFactory, ID2D1DeviceContext4 *pRT,
			  ID2D1Brush *pOutlineBrush, ID2D1Brush *pFillBrush, float outlineSize,
			  bool colorFonts);

	~OBSTextRenderer();

	IFACEMETHOD(IsPixelSnappingDisabled)
	(__maybenull void *clientDrawingContext, __out BOOL *isDisabled);

	IFACEMETHOD(GetCurrentTransform)
	(__maybenull void *clientDrawingContext, __out DWRITE_MATRIX *transform);

	IFACEMETHOD(GetPixelsPerDip)
	(__maybenull void *clientDrawingContext, __out FLOAT *pixelsPerDip);

	IFACEMETHOD(DrawGlyphRun)
	(__maybenull void *clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY,
	 DWRITE_MEASURING_MODE measuringMode, __in DWRITE_GLYPH_RUN const *glyphRun,
	 __in DWRITE_GLYPH_RUN_DESCRIPTION const *glyphRunDescription, IUnknown *clientDrawingEffect);

	IFACEMETHOD(DrawUnderline)
	(__maybenull void *clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY,
	 __in DWRITE_UNDERLINE const *underline, IUnknown *clientDrawingEffect);

	IFACEMETHOD(DrawStrikethrough)
	(__maybenull void *clientDrawingContext, FLOAT baselineOriginX, FLOAT baselineOriginY,
	 __in DWRITE_STRIKETHROUGH const *strikethrough, IUnknown *clientDrawingEffect);

	IFACEMETHOD(DrawInlineObject)
	(__maybenull void *clientDrawingContext, FLOAT originX, FLOAT originY,
	 IDWriteInlineObject *inlineObject, BOOL isSideways, BOOL isRightToLeft, IUnknown *clientDrawingEffect);

public:
	IFACEMETHOD_(unsigned long, AddRef)();
	IFACEMETHOD_(unsigned long, Release)();
	IFACEMETHOD(QueryInterface)(IID const &riid, void **ppvObject);

private:
	unsigned long cRefCount_;
	float outlineSize;
	bool colorFonts;
	ComPtr<IDWriteFactory4> pDWriteFactory;
	ComPtr<ID2D1Factory> pD2DFactory;
	ComPtr<ID2D1DeviceContext4> pDeviceContext;
	ComPtr<ID2D1Brush> pOutlineBrush = nullptr;
	ComPtr<ID2D1Brush> pFillBrush;
	ComPtr<ID2D1SolidColorBrush> pTempBrush;
};
