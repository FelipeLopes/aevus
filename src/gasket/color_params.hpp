#pragma once

#include "palette.hpp"

namespace gasket {

struct ColorParams {
    Palette palette;
    std::vector<double> colorValues;
};

}