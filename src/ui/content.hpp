#pragma once

#include <array>
#include <vector>
#include "../core/custom_coefs.hpp"

namespace ui {

struct CoefsContent {
    double ox, oy, xx, xy, yx, yy;
};

struct XFormTriangleContent {
    int activeId;
    std::vector<CoefsContent> coefs;
};

struct WeightsContent {
    int activeId;
    bool flameLoaded;
    std::vector<double> weights;
};

}