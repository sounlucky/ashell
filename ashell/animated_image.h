#pragma once
#include <functional>
#include <chrono>
#include "layer.h"
#include "animated_image_helpers.h"

//actual class
class animated_image :
    public layer
{
    progress _progress;
public:
    template <class duration_t>
    animated_image(std::chrono::duration<duration_t> duration, RECT rect) : 
        layer(std::move(rect)), _progress(duration) {}

    virtual void drawing_based_on_animation_state(HBITMAP, value_modifier&&) = 0;

    virtual void on_start();
    virtual void on_finish();
    void animated_image::apply(HDC) override;
    virtual ~animated_image() {}
};
