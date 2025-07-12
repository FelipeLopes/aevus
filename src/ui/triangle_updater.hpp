#pragma once

#include "../core/flame.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"

namespace ui {

class TriangleUpdater {
public:
    TriangleUpdater(TriangleGrid* triangleGrid, int activeTransform);
    void handleActiveXformChanged(int id);
    void setFlame(core::Flame* flame);
    UpdateState getUpdateState();
    void startGridDrag(WindowPoint clickPoint);
    void startTriangleDrag(WindowPoint clickPoint);
    void startXDrag(WindowPoint clickPoint);
    void startYDrag(WindowPoint clickPoint);
    void startTriangleRotation(WindowPoint clickPoint);
    void startTriangleScaling(WindowPoint clickPoint);
    void setUpdatePoint(WindowPoint mousePoint);
    void finishUpdate();
private:
    double distancePointLine(GridPoint p, GridPoint s1, GridPoint s2);
    core::Flame* flame;
    TriangleGrid* triangleGrid;
    int activeTransform;
    UpdateState state;
    wxAffineMatrix2D gridDragInverse;
    GridPoint startPoint, startO, startX, startY, startCenter;
};

}