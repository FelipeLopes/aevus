#pragma once

#include "../render/palette.hpp"

namespace gasket {

struct ColorParams {
    render::Palette palette;
    std::vector<double> colorValues;
};

}