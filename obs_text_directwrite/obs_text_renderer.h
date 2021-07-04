#include <dwrite.h>
#include <d2d1.h>
#include <d3d11.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wrl.h>

template<class T> void SafeRelease(T **ppT)
{
	if (*ppT) {
		(*ppT)->Release();
		*ppT = NULL;
	}
}

class obs_text_renderer : public IDWriteTextRenderer {
public:
	obs_text_renderer(IDWriteFactory4 *pDWriteFactory_,
			  ID2D1Factory *pD2DFactory, ID2D1DeviceContext4 *pRT,
			  ID2D1Brush *pOutlineBrush, ID2D1Brush *pFillBrush,
			  float Outline_size_);

	~obs_text_renderer();

	IFACEMETHOD(IsPixelSnappingDisabled)
	(__maybenull void *clientDrawingContext, __out BOOL *isDisabled);

	IFACEMETHOD(GetCurrentTransform)
	(__maybenull void *clientDrawingContext,
	 __out DWRITE_MATRIX *transform);

	IFACEMETHOD(GetPixelsPerDip)
	(__maybenull void *clientDrawingContext, __out FLOAT *pixelsPerDip);

	IFACEMETHOD(DrawGlyphRun)
	(__maybenull void *clientDrawingContext, FLOAT baselineOriginX,
	 FLOAT baselineOriginY, DWRITE_MEASURING_MODE measuringMode,
	 __in DWRITE_GLYPH_RUN const *glyphRun,
	 __in DWRITE_GLYPH_RUN_DESCRIPTION const *glyphRunDescription,
	 IUnknown *clientDrawingEffect);

	IFACEMETHOD(DrawUnderline)
	(__maybenull void *clientDrawingContext, FLOAT baselineOriginX,
	 FLOAT baselineOriginY, __in DWRITE_UNDERLINE const *underline,
	 IUnknown *clientDrawingEffect);

	IFACEMETHOD(DrawStrikethrough)
	(__maybenull void *clientDrawingContext, FLOAT baselineOriginX,
	 FLOAT baselineOriginY, __in DWRITE_STRIKETHROUGH const *strikethrough,
	 IUnknown *clientDrawingEffect);

	IFACEMETHOD(DrawInlineObject)
	(__maybenull void *clientDrawingContext, FLOAT originX, FLOAT originY,
	 IDWriteInlineObject *inlineObject, BOOL isSideways, BOOL isRightToLeft,
	 IUnknown *clientDrawingEffect);

public:
	IFACEMETHOD_(unsigned long, AddRef)();
	IFACEMETHOD_(unsigned long, Release)();
	IFACEMETHOD(QueryInterface)(IID const &riid, void **ppvObject);

private:
	unsigned long cRefCount_;
	float outlineSize;
	IDWriteFactory4 *pDWriteFactory;
	ID2D1Factory *pD2DFactory;
	ID2D1DeviceContext4 *pDeviceContext;
	ID2D1Brush *pOutlineBrush = nullptr;
	ID2D1Brush *pFillBrush;
	ID2D1SolidColorBrush *pTempBrush;
};
