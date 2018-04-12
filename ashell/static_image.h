#pragma once
#include <string>
#include <d2d1.h>
#include "layer.h"
#include "msdn_releaser.h"

//the class will free bitmap itself (:
class static_image :
    public layer
{    
    std::unique_ptr<ID2D1Bitmap, void(*)(ID2D1Bitmap*)> p_bitmap;
public:
    static_image(std::wstring, std::unique_ptr<ID2D1HwndRenderTarget>&, POINT);
    static_image(std::wstring, std::unique_ptr<ID2D1HwndRenderTarget>&);
    void static_image::apply(std::unique_ptr<ID2D1HwndRenderTarget>& render_target) override;
};
