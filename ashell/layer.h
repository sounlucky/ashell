#pragma once
#include <windows.h>
#include <d2d1.h>
#include <memory>
#include "d2d1_object_wrapper.h"

class layer
{
protected:
    RECT area;
public:
    layer(RECT&&);
    virtual void apply(d2d1_unique_object<ID2D1HwndRenderTarget>&) = 0;
    virtual ~layer();
};
