#pragma once

#include <vector>

namespace ui {

struct XformTrianglePoint {
    double x, y;
};

struct XformTriangle {
    XformTrianglePoint o, x, y;
};

struct XformTriangleContent {
    int activeId;
    std::vector<XformTriangle> triangles;
};

}