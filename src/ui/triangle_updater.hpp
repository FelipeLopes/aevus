#pragma once

#include "../core/flame.hpp"
#include "content.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"

namespace ui {

class TriangleUpdater {
public:
    TriangleUpdater(TriangleGrid* triangleGrid);
    void handleContent(const XFormTriangleContent& content);
    UpdateState getUpdateState();
    void startGridDrag(WindowPoint clickPoint);
    void startTriangleDrag(WindowPoint clickPoint);
    void startXDrag(WindowPoint clickPoint);
    void startYDrag(WindowPoint clickPoint);
    void startTriangleRotation(WindowPoint clickPoint);
    void startTriangleScaling(WindowPoint clickPoint);
    void updateCenterForPoint(WindowPoint mousePoint);
    CoefsContent getCoefsForPoint(WindowPoint mousePoint);
    void finishUpdate();
private:
    void setUpdateStartData(WindowPoint clickPoint);
    double distancePointLine(GridPoint p, GridPoint s1, GridPoint s2);
    TriangleGrid* triangleGrid;
    XFormTriangleContent content;
    UpdateState state;
    wxAffineMatrix2D gridDragInverse;
    GridPoint startPoint, startO, startX, startY, startCenter;
};

}