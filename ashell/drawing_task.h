#pragma once

#include <gdiplus.h>

class drawing_task
{
    Gdiplus::Graphics& graphics;
public:
    drawing_task(Gdiplus::Graphics& graphics) : graphics(graphics) {}
    virtual ~drawing_task() {}

    virtual bool perform() = 0;//returns false in case its expired
};

