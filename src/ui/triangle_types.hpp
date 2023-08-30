#pragma once

#include <wx/geometry.h>

namespace ui {

class GridPoint: public wxPoint2DDouble {
public:
    GridPoint(): wxPoint2DDouble(0.0, 0.0) { }
    GridPoint(double x, double y): wxPoint2DDouble(x, y) { }
    GridPoint(wxPoint2DDouble p): wxPoint2DDouble(p.m_x, p.m_y) { }
    GridPoint(std::pair<double, double> p): wxPoint2DDouble(p.first, p.second) { }
};

class WindowPoint: public wxPoint2DDouble {
public:
    WindowPoint(): wxPoint2DDouble(0.0, 0.0) { }
    WindowPoint(double x, double y): wxPoint2DDouble(x, y) { }
    WindowPoint(wxPoint2DDouble p): wxPoint2DDouble(p.m_x, p.m_y) { }
    WindowPoint(std::pair<double, double> p): wxPoint2DDouble(p.first, p.second) { }
};

enum CollisionType {
    NO_COLLISION,
    TRIANGLE_BODY,
    VERTEX_O,
    VERTEX_X,
    VERTEX_Y,
    EDGE_OX,
    EDGE_OY,
    EDGE_XY
};

struct Collision {
    int triangleId;
    CollisionType type;
    Collision() {
        triangleId = -1;
        type = NO_COLLISION;
    }
};

enum UpdaterState {
    NO_UPDATE,
    DRAGGING_GRID,
    DRAGGING_TRIANGLE,
    DRAGGING_X,
    DRAGGING_Y,
    ROTATING_TRIANGLE,
    SCALING_TRIANGLE
};

}