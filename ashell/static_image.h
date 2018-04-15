#pragma once
#include <string>
#include <d2d1.h>
#include "layer.h"
#include "d2d1_object_wrapper.h"

//the class will free bitmap itself (:
class static_image :
    public layer
{    
    d2d1_unique_object<ID2D1Bitmap> p_bitmap;
public:
    static_image(std::wstring, d2d1_unique_object<ID2D1HwndRenderTarget>&, POINT);
    static_image(std::wstring, d2d1_unique_object<ID2D1HwndRenderTarget>&);
    void static_image::apply(d2d1_unique_object<ID2D1HwndRenderTarget>& render_target) override;
};
