#include <algorithm>
#include <memory>
#include <cassert>
#include <windows.h>
#include <gdiplus.h>
#include "static_image.h"

static_image::static_image(std::wstring path, POINT origin): static_image(path) {
    this->origin = origin;
}

static_image::static_image(std::wstring path): 
    layer({0,0,0,0}){

    using namespace Gdiplus;

    std::unique_ptr<Bitmap> gdiplus_bitmap(Bitmap::FromFile(path.c_str()));

    area.bottom = gdiplus_bitmap->GetHeight();
    area.right = gdiplus_bitmap->GetWidth();

    Status s1 = gdiplus_bitmap.get()->GetHBITMAP(Color::Transparent, &h_bitmap);
    assert(s1 == Ok);
}

void static_image::apply(HDC hdc) {
    auto temp_dc = CreateCompatibleDC(hdc);
    SelectObject(temp_dc, h_bitmap);
    auto res = TransparentBlt(
        hdc,
        origin.x, origin.y, 
        area.right - area.left, area.bottom - area.top,
        temp_dc, 
        0, 0, 
        area.right, area.bottom,
        Gdiplus::Color::Transparent
    );
    assert(res);
    DeleteDC(temp_dc);
}

static_image::~static_image(){
    DeleteObject(h_bitmap);
}
