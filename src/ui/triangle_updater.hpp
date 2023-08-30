#pragma once

#include <memory>
#include "../core/flame.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"

namespace ui {

class TriangleUpdater {
public:
    TriangleUpdater(std::shared_ptr<core::Flame> flame, std::shared_ptr<TriangleGrid> triangleGrid);
    bool updating();
    void startGridDrag(WindowPoint clickPoint);
    void startTriangleDrag(WindowPoint clickPoint);
    void startXDrag(WindowPoint clickPoint);
    void startYDrag(WindowPoint clickPoint);
    void startTriangleRotation(WindowPoint clickPoint);
    void startTriangleScaling(WindowPoint clickPoint);
    void setUpdatePoint(WindowPoint mousePoint);
    void finishUpdate();
private:
    std::shared_ptr<core::Flame> flame;
    std::shared_ptr<TriangleGrid> triangleGrid;
    UpdaterState state;
};

}