#include <algorithm>
#include <memory>
#include <cassert>
#include <windows.h>
#include <Wincodec.h>
#include "static_image.h"

static_image::static_image(std::wstring path, d2d1_unique_object<ID2D1HwndRenderTarget>& render_target, POINT origin):
    static_image(path, render_target)
{
    area.bottom += origin.y;
    area.left += origin.x;
    area.right += origin.x;
    area.top += origin.y;
}

static_image::static_image(std::wstring path, d2d1_unique_object<ID2D1HwndRenderTarget>& render_target):
    layer({0,0,0,0})
{
    d2d1_unique_object<IWICImagingFactory> factory = nullptr;
    d2d1_unique_object<IWICBitmapDecoder> decoder = nullptr;
    d2d1_unique_object<IWICFormatConverter> converter = nullptr;
    d2d1_unique_object<IWICBitmapFrameDecode> bmp_src = nullptr;

    CoInitialize(nullptr);
    {
        void* temp = factory.get();
        auto hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
            __uuidof(IWICImagingFactory), &temp);
        assert(SUCCEEDED(hr));
        factory.reset(reinterpret_cast<IWICImagingFactory*>(temp));
    }
    {
        auto temp = decoder.get();
        auto hr = factory->CreateDecoderFromFilename(path.c_str(), nullptr, GENERIC_READ,
            WICDecodeMetadataCacheOnLoad, &temp);
        assert(SUCCEEDED(hr));
        decoder.reset(temp);
    }
    {
        auto temp = converter.get();
        auto hr = factory->CreateFormatConverter(&temp);
        assert(SUCCEEDED(hr));
        converter.reset(temp);
    }
    {
        auto temp = bmp_src.get();
        auto hr = decoder->GetFrame(0, &temp);
        assert(SUCCEEDED(hr));
        bmp_src.reset(temp);
    } 
    {
        auto hr = converter->Initialize(bmp_src.get(), GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
        assert(SUCCEEDED(hr));
    }
    {
        auto temp = p_bitmap.get();
        auto hr = render_target->CreateBitmapFromWicBitmap(converter.get(), nullptr, &temp);
        assert(SUCCEEDED(hr));
        p_bitmap.reset(temp);
    }
    //filling area
    auto sz = p_bitmap->GetPixelSize();
    area.bottom += sz.height;
    area.right += sz.width;
}

void static_image::apply(d2d1_unique_object<ID2D1HwndRenderTarget>& render_target) {
    render_target->DrawBitmap(p_bitmap.get(), //does not work, todo
        { static_cast<float>(area.left), static_cast<float>(area.top),
        static_cast<float>(area.right), static_cast<float>(area.bottom) });
}
