#include <cassert>
#include "layer_hierarchy.h"

void layer_hierarchy::add_layer(layer* raw_ptr){
    std::unique_ptr<layer> unique_one(raw_ptr);
    layers.push_back(std::move(unique_one));
}

void layer_hierarchy::apply(d2d1_unique_object<ID2D1HwndRenderTarget>& rt){
    for (layer_container::size_type i = 0; i < layers.size(); ++i) {
        auto& layer = layers[i];
        assert(layer.get());
        layer->apply(rt);
    }
}
