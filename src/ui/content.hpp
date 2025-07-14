#pragma once

#include <array>
#include <vector>
#include "../core/custom_coefs.hpp"

namespace ui {

struct XFormTriangle {
    std::array<std::pair<double, double>,3> arr;
    core::CoefsAffine coefs;
};

struct XFormTriangleContent {
    int activeId;
    std::vector<XFormTriangle> triangles;
};

struct CoefsContent {
    double ox, oy, xx, xy, yx, yy;
};

}