#include "layer.h"
#include <algorithm>

layer::layer(RECT&& in, POINT p) :
    area(in), origin(std::move(p)) {}

layer::~layer() {}
