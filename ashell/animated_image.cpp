#include "animated_image.h"

void apply(std::unique_ptr<ID2D1HwndRenderTarget>&) {

}

void animated_image::on_start() {}

void animated_image::on_finish() {
    done = true; 
}
