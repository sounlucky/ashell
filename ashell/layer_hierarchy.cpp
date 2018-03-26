#include <cassert>
#include "layer_hierarchy.h"

void layer_hierarchy::add_layer(layer* raw_ptr){
    std::unique_ptr<layer> unique_one(raw_ptr);
    layers.push_back(std::move(unique_one));
}

void layer_hierarchy::apply(HDC hdc){
    for (layer_container::size_type i = 0; i < layers.size(); ) {
        auto& layer = layers[i];
        assert(layer.get());
        layer->apply(hdc);
        if (layer->is_done()) {
            layers.erase(layers.begin() + i);
        }
        else {
            ++i;
        }
    }
}
