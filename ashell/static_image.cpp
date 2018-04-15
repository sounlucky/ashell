#include <algorithm>
#include <memory>
#include <cassert>
#include <windows.h>
#include <Wincodec.h>
#include "static_image.h"

static_image::static_image(std::wstring path, std::unique_ptr<ID2D1HwndRenderTarget>& render_target, POINT origin):
    static_image(path, render_target)
{
    this->origin = origin;
}

static_image::static_image(std::wstring path, std::unique_ptr<ID2D1HwndRenderTarget>& render_target):
    layer({0,0,0,0})
{
    IWICImagingFactory* d2dWICFactory = nullptr;
    IWICBitmapDecoder* d2dDecoder = nullptr;
    IWICFormatConverter* d2dConverter = nullptr;
    IWICBitmapFrameDecode* d2dBmpSrc = nullptr;
    ID2D1Bitmap* d2dBmp = nullptr;

    CoInitialize(nullptr);

    auto hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
        __uuidof(IWICImagingFactory), (void**)(&d2dWICFactory));
    assert(SUCCEEDED(hr));
    hr = d2dWICFactory->CreateDecoderFromFilename(path.c_str(), NULL, GENERIC_READ,
        WICDecodeMetadataCacheOnLoad, &d2dDecoder);
    assert(SUCCEEDED(hr));
    hr = d2dWICFactory->CreateFormatConverter(&d2dConverter);
    assert(SUCCEEDED(hr));
    hr = d2dDecoder->GetFrame(0, &d2dBmpSrc);
    assert(SUCCEEDED(hr));
    hr = d2dConverter->Initialize(d2dBmpSrc, GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
    assert(SUCCEEDED(hr));
    hr = render_target->CreateBitmapFromWicBitmap(d2dConverter, NULL, &d2dBmp);
    assert(SUCCEEDED(hr));
    p_bitmap.reset(d2dBmp);
    
    //filling area
    auto sz = p_bitmap->GetPixelSize();
    area.bottom = sz.height;
    area.right = sz.width;

    //releasing stuff
    SafeRelease(&d2dWICFactory);
    SafeRelease(&d2dDecoder);
    SafeRelease(&d2dConverter);
    SafeRelease(&d2dBmpSrc);
}

void static_image::apply(std::unique_ptr<ID2D1HwndRenderTarget>& render_target) {
    render_target->DrawBitmap(p_bitmap.get(), //does not work, todo
        { static_cast<float>(origin.x + area.left), static_cast<float>(origin.y + area.top),
        static_cast<float>(origin.x + area.right), static_cast<float>(origin.x + area.bottom) });
}
