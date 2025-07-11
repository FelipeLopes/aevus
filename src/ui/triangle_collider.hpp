#pragma once

#include "../core/flame.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"

namespace ui {

class TriangleCollider {
public:
    TriangleCollider(core::Flame* flame, TriangleGrid* triangleGrid, int activeTransform);
    Collision getCollision(WindowPoint pos);
    void handleActiveXformChanged(int id);
    void setFlame(core::Flame* flame);
private:
    CollisionType getCollisionType(WindowPoint pos, int triangle);
    int checkVertexCollision(WindowPoint p, int idx);
    int checkEdgeCollision(WindowPoint p, int idx);
    bool pointInsideTriangle(GridPoint p, int idx);
    double distancePointSegment(WindowPoint p, WindowPoint s1, WindowPoint s2);
    double sign(GridPoint p1, GridPoint p2, GridPoint p3);

    core::Flame* flame;
    TriangleGrid* triangleGrid;
    int activeTransform;
};

}