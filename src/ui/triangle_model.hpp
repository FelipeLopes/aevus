#pragma once

#include <memory>
#include "../core/flame.hpp"
#include "triangle_collider.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"
#include <boost/signals2.hpp>
#include <wx/graphics.h>
#include <wx/event.h>
#include <wx/affinematrix2d.h>
#include <wx/panel.h>

namespace ui {

class TriangleModel {
public:
    TriangleModel(std::shared_ptr<core::Flame> flame, wxPanel* trianglePanel);
    void update();
    void handleActiveXformChanged(int id);
    void handlePaint();
    void handleResize(wxSizeEvent& event);
    void handleMouseWheel(wxMouseEvent& event);
    void handleMouseUp(wxMouseEvent& event);
    void handleMouseDown(wxMouseEvent& event);
    void handleMouseMove(wxMouseEvent& event);

    boost::signals2::signal<void (int)> xformSelected;
    boost::signals2::signal<void ()> transformCoordsChanged;
private:
    void drawXformTriangles(wxGraphicsContext* gc);
    void drawTriangleDots(wxGraphicsContext* gc, wxColour color,
        const std::vector<GridPoint>& triangle);
    void drawDot(wxGraphicsContext* gc, double x, double y, std::string label);
    std::vector<GridPoint> getXformTriangle(int i);
    void highlightTriangle(wxGraphicsContext* gc, int i);
    bool pointInsideTriangle(wxPoint2DDouble p, int idx);
    double distancePointSegment(wxPoint2DDouble p, wxPoint2DDouble s1, wxPoint2DDouble s2);
    double distancePointLine(wxPoint2DDouble p, wxPoint2DDouble s1, wxPoint2DDouble s2);
    double sign(wxPoint2DDouble p1, wxPoint2DDouble p2, wxPoint2DDouble p3);
    Collision getCollision(wxPoint pos);
    CollisionType getCollisionType(wxPoint pos, int triangle);
    int checkVertexCollision(wxPoint p, int idx);
    int checkEdgeCollision(wxPoint p, int idx);
    void highlightVertex(wxGraphicsContext* gc, int triangle, int vertex);
    void highlightEdge(wxGraphicsContext* gc, int triangle, int edge);

    std::shared_ptr<core::Flame> flame;
    std::shared_ptr<TriangleGrid> triangleGrid;
    std::shared_ptr<TriangleCollider> triangleCollider;
    wxPanel* trianglePanel;
    int activeTransform;
    bool draggingGrid;
    wxAffineMatrix2D dragInverseAffine;
    wxPoint2DDouble dragBegin, centerDragStart;
    std::vector<wxColour> xformColors;
    std::vector<std::string> dotLabels;
    int highlightedTriangle;
    bool draggingTriangle;
    wxPoint2DDouble originDragStart;
    CollisionType highlightType;
    bool draggingX, draggingY, rotatingTriangle, scalingTriangle;
    wxPoint2DDouble xDragStart, yDragStart;
};

}