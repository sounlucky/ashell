#pragma once
#include <windows.h>
#include <d2d1.h>
#include <memory>
class layer
{
protected:
    RECT area;
    POINT origin;
    bool done = false;//set done to true to delete layer after "applying" it
public:
    layer(RECT&&, POINT p = {0,0});
    bool is_done() const { return done; }
    virtual void apply(std::unique_ptr<ID2D1HwndRenderTarget>&) = 0;
    virtual ~layer();
};
