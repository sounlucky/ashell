#pragma once
#include <vector>
#include <memory>
#include "layer.h"
#include "d2d1_object_wrapper.h"

class layer_hierarchy
{
    using layer_container = std::vector<std::unique_ptr<layer>>;
    layer_container layers;
public:
    layer_hierarchy() = default;
    void add_layer(layer*);//makes ptr unique, will free memory itself
    void apply(d2d1_unique_object<ID2D1HwndRenderTarget>&);
};

