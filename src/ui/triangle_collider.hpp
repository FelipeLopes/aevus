#pragma once

#include "../core/flame.hpp"
#include "content.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"

namespace ui {

class TriangleCollider {
public:
    TriangleCollider(TriangleGrid* triangleGrid);
    Collision getCollision(WindowPoint pos);
    void handleContent(std::optional<XFormTriangleContent> content);
    void handleActiveXformContent(ActiveXFormContent xformContent);
private:
    CollisionType getCollisionType(WindowPoint pos, int triangle);
    int checkVertexCollision(WindowPoint p, int idx);
    int checkEdgeCollision(WindowPoint p, int idx);
    bool pointInsideTriangle(GridPoint p, int idx);
    double distancePointSegment(WindowPoint p, WindowPoint s1, WindowPoint s2);
    double sign(GridPoint p1, GridPoint p2, GridPoint p3);

    TriangleGrid* triangleGrid;
    std::optional<XFormTriangleContent> content;
};

}