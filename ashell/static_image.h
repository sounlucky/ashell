#pragma once
#include <string>
#include "layer.h"

//the class will free bitmap itself (:
class static_image :
    public layer
{    public://remove
    HBITMAP h_bitmap;
public:
    static_image(std::wstring);
    static_image(std::wstring, POINT);
    void static_image::apply(HDC) override;
    ~static_image();
};
