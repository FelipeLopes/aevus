#pragma once

#include <memory>
#include "../core/flame.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"

namespace ui {

class TriangleCollider {
public:
    TriangleCollider(std::shared_ptr<core::Flame>, std::shared_ptr<TriangleGrid>,
        int activeTransform);
    Collision getCollision(WindowPoint pos);
    void handleActiveXformChanged(int id);
private:
    CollisionType getCollisionType(WindowPoint pos, int triangle);
    int checkVertexCollision(WindowPoint p, int idx);
    int checkEdgeCollision(WindowPoint p, int idx);
    bool pointInsideTriangle(GridPoint p, int idx);
    std::vector<GridPoint> getXformTriangle(int i);
    double distancePointSegment(WindowPoint p, WindowPoint s1, WindowPoint s2);
    double sign(GridPoint p1, GridPoint p2, GridPoint p3);

    std::shared_ptr<core::Flame> flame;
    std::shared_ptr<TriangleGrid> triangleGrid;
    int activeTransform;
};

}