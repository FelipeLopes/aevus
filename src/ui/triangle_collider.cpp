#include "triangle_collider.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"

using core::Flame;

namespace ui {

TriangleCollider::TriangleCollider(TriangleGrid* triangleGrid_,
    int activeTransform_): flame(NULL), triangleGrid(triangleGrid_),
    activeTransform(activeTransform_) { }

Collision TriangleCollider::getCollision(WindowPoint pos) {
    Collision ans;
    ans.triangleId = -1;
    if (activeTransform == -1) {
        ans.type = NO_COLLISION;
        return ans;
    }
    ans.type = getCollisionType(pos, activeTransform);
    if (ans.type == NO_COLLISION) {
        for (int i=0; i<flame->xforms.size(); i++) {
            if (i == activeTransform) {
                continue;
            }
            ans.type = getCollisionType(pos, i);
            if (ans.type != NO_COLLISION) {
                ans.triangleId = i;
                break;
            }
        }
    } else {
        ans.triangleId = activeTransform;
    }
    return ans;
}

void TriangleCollider::handleActiveXformChanged(int id) {
    activeTransform = id;
}

void TriangleCollider::setFlame(core::Flame* flame) {
    this->flame = flame;
}

CollisionType TriangleCollider::getCollisionType(WindowPoint pos, int triangle) {
    auto ans = NO_COLLISION;
    int vertex = checkVertexCollision(pos, triangle);
    auto tp = triangleGrid->transformToGrid(pos);
    if (vertex != -1) {
        switch (vertex) {
            case 0: ans = VERTEX_O; break;
            case 1: ans = VERTEX_X; break;
            case 2: ans = VERTEX_Y; break;
        }
    } else {
        int edge = checkEdgeCollision(pos, triangle);
        if (edge != -1) {
            switch (edge) {
                case 0: ans = EDGE_OX; break;
                case 1: ans = EDGE_XY; break;
                case 2: ans = EDGE_OY; break;
            }
        } else if (pointInsideTriangle(tp, triangle)) {
            ans = TRIANGLE_BODY;
        }
    }
    return ans;
}

int TriangleCollider::checkVertexCollision(WindowPoint p, int idx) {
    auto triangle = flame->xforms.get(idx)->coefs.value().triangle();
    // We check in order X -> Y -> O, because the X and Y vertices
    // are better when the triangle collapses to a point.
    for (int i=0; i<3; i++) {
        auto tp = triangleGrid->transformToWindow(GridPoint(triangle[(i+1)%3]));
        if (tp.GetDistance(p) < 5) {
            return (i+1)%3;
        }
    }
    return -1;
}

int TriangleCollider::checkEdgeCollision(WindowPoint p, int idx) {
    auto triangle = flame->xforms.get(idx)->coefs.value().triangle();
    // We check XY last to avoid problems when the triangle
    // degenerates to a line.
    for (int i=0; i<3; i++) {
        auto s1 = triangleGrid->transformToWindow(GridPoint(triangle[(i+2)%3]));
        auto s2 = triangleGrid->transformToWindow(GridPoint(triangle[i%3]));
        if (distancePointSegment(p, s1, s2) < 3) {
            return (i+2)%3;
        }
    }
    return -1;
}

bool TriangleCollider::pointInsideTriangle(GridPoint p, int idx) {
    auto v = flame->xforms.get(idx)->coefs.value().triangle();
    double d1 = sign(p, GridPoint(v[0]), GridPoint(v[1]));
    double d2 = sign(p, GridPoint(v[1]), GridPoint(v[2]));
    double d3 = sign(p, GridPoint(v[2]), GridPoint(v[0]));
    bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(hasNeg && hasPos);
}

double TriangleCollider::distancePointSegment(WindowPoint p, WindowPoint s1, WindowPoint s2) {
    double d2 = s1.GetDistanceSquare(s2);
    if (d2 == 0.0) {
        return p.GetDistance(s1);
    }
    double t = (p-s1).GetDotProduct(s2-s1)/d2;
    t = std::clamp(t, 0.0, 1.0);
    auto proj = s1 + t*(s2-s1);
    return p.GetDistance(proj);
}

double TriangleCollider::sign(GridPoint p1, GridPoint p2, GridPoint p3) {
    return (p1.m_x-p3.m_x)*(p2.m_y-p3.m_y)-(p2.m_x-p3.m_x)*(p1.m_y-p3.m_y);
}

}