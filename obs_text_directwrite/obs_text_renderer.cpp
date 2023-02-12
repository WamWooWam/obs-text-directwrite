

#include "obs_text_renderer.h"

using namespace Microsoft::WRL;

OBSTextRenderer::OBSTextRenderer(IDWriteFactory2 *pDWriteFactory_, ID2D1Factory *pD2DFactory_,
				 ID2D1DeviceContext1 *pDeviceContext_, ID2D1Brush *pOutlineBrush_,
				 ID2D1Brush *pFillBrush_, float outlineSize_, bool colorFonts_)
	: cRefCount_(0),
	  pD2DFactory(pD2DFactory_),
	  pDWriteFactory(pDWriteFactory_),
	  pDeviceContext(pDeviceContext_),
	  pFillBrush(pFillBrush_),
	  pOutlineBrush(pOutlineBrush_),
	  outlineSize(outlineSize_),
	  colorFonts(colorFonts_)
{
	pDeviceContext_->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), pTempBrush.GetAddressOf());
	pDWriteFactory_->QueryInterface<IDWriteFactory4>(pDWriteFactory4.ReleaseAndGetAddressOf());
	pDeviceContext_->QueryInterface<ID2D1DeviceContext4>(pDeviceContext4.ReleaseAndGetAddressOf());
}

OBSTextRenderer::~OBSTextRenderer() {}

IFACEMETHODIMP OBSTextRenderer::DrawGlyphRun(__maybenull void *clientDrawingContext, FLOAT baselineOriginX,
					     FLOAT baselineOriginY, DWRITE_MEASURING_MODE measuringMode,
					     __in DWRITE_GLYPH_RUN const *glyphRun,
					     __in DWRITE_GLYPH_RUN_DESCRIPTION const *glyphRunDescription,
					     IUnknown *clientDrawingEffect)
{
	HRESULT hr = DWRITE_E_NOCOLOR;
	D2D1_POINT_2F baselineOrigin = D2D1::Point2F(baselineOriginX, baselineOriginY);

	if (pOutlineBrush) {
		ComPtr<ID2D1PathGeometry> pGeometry;
		ComPtr<ID2D1GeometrySink> pSink;
		ComPtr<ID2D1TransformedGeometry> pTransformedGeometry;
		pD2DFactory->CreatePathGeometry(pGeometry.GetAddressOf());
		pGeometry->Open(pSink.GetAddressOf());

		glyphRun->fontFace->GetGlyphRunOutline(glyphRun->fontEmSize, glyphRun->glyphIndices,
						       glyphRun->glyphAdvances, glyphRun->glyphOffsets,
						       glyphRun->glyphCount, glyphRun->isSideways,
						       glyphRun->bidiLevel % 2, pSink.Get());

		pSink->Close();

		D2D1::Matrix3x2F const matrix =
			D2D1::Matrix3x2F(1.0f, 0.0f, 0.0f, 1.0f, baselineOriginX, baselineOriginY);

		pD2DFactory->CreateTransformedGeometry(pGeometry.Get(), &matrix,
						       pTransformedGeometry.GetAddressOf());

		pDeviceContext->DrawGeometry(pTransformedGeometry.Get(), pOutlineBrush.Get(), outlineSize);
	}

	// The list of glyph image formats this renderer is prepared to support.
	DWRITE_GLYPH_IMAGE_FORMATS supportedFormats =
		DWRITE_GLYPH_IMAGE_FORMATS_TRUETYPE | DWRITE_GLYPH_IMAGE_FORMATS_CFF |
		DWRITE_GLYPH_IMAGE_FORMATS_COLR | DWRITE_GLYPH_IMAGE_FORMATS_SVG |
		DWRITE_GLYPH_IMAGE_FORMATS_PNG | DWRITE_GLYPH_IMAGE_FORMATS_JPEG |
		DWRITE_GLYPH_IMAGE_FORMATS_TIFF | DWRITE_GLYPH_IMAGE_FORMATS_PREMULTIPLIED_B8G8R8A8;

	// Determine whether there are any color glyph runs within glyphRun. If
	// there are, glyphRunEnumerator can be used to iterate through them.
	ComPtr<IDWriteColorGlyphRunEnumerator> glyphRunEnumerator;
	ComPtr<IDWriteColorGlyphRunEnumerator1> glyphRunEnumerator1;
	if (pDWriteFactory4) {
		hr = pDWriteFactory4->TranslateColorGlyphRun(baselineOrigin, glyphRun, glyphRunDescription,
							     supportedFormats, measuringMode, nullptr, 0,
							     glyphRunEnumerator1.GetAddressOf());
	} else {
		hr = pDWriteFactory->TranslateColorGlyphRun(baselineOriginX, baselineOriginY, glyphRun,
							    glyphRunDescription, measuringMode, nullptr, 0,
							    glyphRunEnumerator.GetAddressOf());
	}

	if (hr == DWRITE_E_NOCOLOR || !colorFonts) {
		// Simple case: the run has no color glyphs. Draw the main glyph run
		// using the current text color.
		pDeviceContext->DrawGlyphRun(baselineOrigin, glyphRun, glyphRunDescription, pFillBrush.Get(),
					     measuringMode);
	} else {
		//DX::ThrowIfFailed(hr);

		// Complex case: the run has one or more color runs within it. Iterate
		// over the sub-runs and draw them, depending on their format.
		for (;;) {
			BOOL haveRun = FALSE;

			if (glyphRunEnumerator1) {
				glyphRunEnumerator1->MoveNext(&haveRun);
				if (!haveRun)
					break;

				const DWRITE_COLOR_GLYPH_RUN1 *colorRun;
				glyphRunEnumerator1->GetCurrentRun(&colorRun);
				D2D1_POINT_2F currentBaselineOrigin =
					D2D1::Point2F(colorRun->baselineOriginX, colorRun->baselineOriginY);

				switch (colorRun->glyphImageFormat) {
				case DWRITE_GLYPH_IMAGE_FORMATS_PNG:
				case DWRITE_GLYPH_IMAGE_FORMATS_JPEG:
				case DWRITE_GLYPH_IMAGE_FORMATS_TIFF:
				case DWRITE_GLYPH_IMAGE_FORMATS_PREMULTIPLIED_B8G8R8A8: {
					// This run is bitmap glyphs. Use Direct2D to draw them.
					pDeviceContext4->DrawColorBitmapGlyphRun(colorRun->glyphImageFormat,
										 currentBaselineOrigin,
										 &colorRun->glyphRun,
										 measuringMode);
				} break;

				case DWRITE_GLYPH_IMAGE_FORMATS_SVG: {
					// This run is SVG glyphs. Use Direct2D to draw them.
					pDeviceContext4->DrawSvgGlyphRun(currentBaselineOrigin,
									 &colorRun->glyphRun, pFillBrush.Get(),
									 nullptr, // svgGlyphStyle
									 0,       // colorPaletteIndex
									 measuringMode);
				} break;

				default: {
					// This run is solid-color outlines, either from non-color
					// glyphs or from COLR glyph layers. Use Direct2D to draw them.
					ComPtr<ID2D1Brush> layerBrush;
					if (colorRun->paletteIndex == 0xFFFF) {
						// This run uses the current text color.
						layerBrush = pFillBrush;
					} else {
						// This run specifies its own color.
						pTempBrush->SetColor(colorRun->runColor);
						layerBrush = pTempBrush;
					}

					// Draw the run with the selected color.
					pDeviceContext->DrawGlyphRun(currentBaselineOrigin, &colorRun->glyphRun,
								     colorRun->glyphRunDescription,
								     layerBrush.Get(), measuringMode);
				} break;
				}
			} else {
				glyphRunEnumerator->MoveNext(&haveRun);
				if (!haveRun)
					break;

				const DWRITE_COLOR_GLYPH_RUN *colorRun;
				glyphRunEnumerator->GetCurrentRun(&colorRun);
				D2D1_POINT_2F currentBaselineOrigin =
					D2D1::Point2F(colorRun->baselineOriginX, colorRun->baselineOriginY);

				// This run is solid-color outlines, either from non-color
				// glyphs or from COLR glyph layers. Use Direct2D to draw them.
				ComPtr<ID2D1Brush> layerBrush;
				if (colorRun->paletteIndex == 0xFFFF) {
					// This run uses the current text color.
					layerBrush = pFillBrush;
				} else {
					// This run specifies its own color.
					pTempBrush->SetColor(colorRun->runColor);
					layerBrush = pTempBrush;
				}

				// Draw the run with the selected color.
				pDeviceContext->DrawGlyphRun(currentBaselineOrigin, &colorRun->glyphRun,
							     colorRun->glyphRunDescription, layerBrush.Get(),
							     measuringMode);
			}
		}
	}

	return hr;
}

IFACEMETHODIMP
OBSTextRenderer::DrawUnderline(__maybenull void *clientDrawingContext, FLOAT baselineOriginX,
			       FLOAT baselineOriginY, __in DWRITE_UNDERLINE const *underline,
			       IUnknown *clientDrawingEffect)
{
	HRESULT hr;

	D2D1_RECT_F rect =
		D2D1::RectF(0, underline->offset, underline->width, underline->offset + underline->thickness);

	ComPtr<ID2D1RectangleGeometry> pRectangleGeometry;
	ComPtr<ID2D1TransformedGeometry> pTransformedGeometry;

	hr = pD2DFactory->CreateRectangleGeometry(&rect, pRectangleGeometry.GetAddressOf());

	D2D1::Matrix3x2F const matrix =
		D2D1::Matrix3x2F(1.0f, 0.0f, 0.0f, 1.0f, baselineOriginX, baselineOriginY);

	if (SUCCEEDED(hr)) {
		hr = pD2DFactory->CreateTransformedGeometry(pRectangleGeometry.Get(), &matrix,
							    pTransformedGeometry.GetAddressOf());
	}

	if (pOutlineBrush)
		pDeviceContext->DrawGeometry(pTransformedGeometry.Get(), pOutlineBrush.Get(), outlineSize);

	pDeviceContext->FillGeometry(pTransformedGeometry.Get(), pFillBrush.Get());

	return S_OK;
}

IFACEMETHODIMP OBSTextRenderer::DrawStrikethrough(__maybenull void *clientDrawingContext, FLOAT baselineOriginX,
						  FLOAT baselineOriginY,
						  __in DWRITE_STRIKETHROUGH const *strikethrough,
						  IUnknown *clientDrawingEffect)
{
	HRESULT hr;

	D2D1_RECT_F rect = D2D1::RectF(0, strikethrough->offset, strikethrough->width,
				       strikethrough->offset + strikethrough->thickness);

	ComPtr<ID2D1RectangleGeometry> pRectangleGeometry;
	ComPtr<ID2D1TransformedGeometry> pTransformedGeometry;

	hr = pD2DFactory->CreateRectangleGeometry(&rect, pRectangleGeometry.GetAddressOf());

	D2D1::Matrix3x2F const matrix =
		D2D1::Matrix3x2F(1.0f, 0.0f, 0.0f, 1.0f, baselineOriginX, baselineOriginY);

	if (SUCCEEDED(hr)) {
		hr = pD2DFactory->CreateTransformedGeometry(pRectangleGeometry.Get(), &matrix,
							    pTransformedGeometry.GetAddressOf());
	}

	if (pOutlineBrush)
		pDeviceContext->DrawGeometry(pTransformedGeometry.Get(), pOutlineBrush.Get(), outlineSize);

	pDeviceContext->FillGeometry(pTransformedGeometry.Get(), pFillBrush.Get());

	return S_OK;
}

IFACEMETHODIMP OBSTextRenderer::DrawInlineObject(__maybenull void *clientDrawingContext, FLOAT originX,
						 FLOAT originY, IDWriteInlineObject *inlineObject,
						 BOOL isSideways, BOOL isRightToLeft,
						 IUnknown *clientDrawingEffect)
{
	//return E_NOTIMPL;

	return inlineObject->Draw(clientDrawingContext, this, originX, originY, isSideways, isRightToLeft,
				  clientDrawingEffect);
}

IFACEMETHODIMP OBSTextRenderer::IsPixelSnappingDisabled(__maybenull void *clientDrawingContext,
							__out BOOL *isDisabled)
{
	*isDisabled = FALSE;
	return S_OK;
}

IFACEMETHODIMP
OBSTextRenderer::GetCurrentTransform(__maybenull void *clientDrawingContext, __out DWRITE_MATRIX *transform)
{
	//forward the render target's transform
	pDeviceContext->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F *>(transform));
	return S_OK;
}

IFACEMETHODIMP
OBSTextRenderer::GetPixelsPerDip(__maybenull void *clientDrawingContext, __out FLOAT *pixelsPerDip)
{
	*pixelsPerDip = 1.0f;
	return S_OK;
}

IFACEMETHODIMP_(unsigned long) OBSTextRenderer::AddRef()
{
	return InterlockedIncrement(&cRefCount_);
}

IFACEMETHODIMP_(unsigned long) OBSTextRenderer::Release()
{
	unsigned long newCount = InterlockedDecrement(&cRefCount_);
	if (newCount == 0) {
		delete this;
		return 0;
	}

	return newCount;
}

IFACEMETHODIMP OBSTextRenderer::QueryInterface(IID const &riid, void **ppvObject)
{
	if (__uuidof(IDWriteTextRenderer) == riid) {
		*ppvObject = this;
	} else if (__uuidof(IDWritePixelSnapping) == riid) {
		*ppvObject = this;
	} else if (__uuidof(IUnknown) == riid) {
		*ppvObject = this;
	} else {
		*ppvObject = NULL;
		return E_FAIL;
	}

	this->AddRef();

	return S_OK;
}
