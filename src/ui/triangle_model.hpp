#pragma once

#include <memory>
#include "../core/flame.hpp"
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
    };

    double calcStepForScale(double sc);
    void setupGrid();
    void drawGrid(wxGraphicsContext* gc);
    void drawXformTriangles(wxGraphicsContext* gc);
    void drawTriangleDots(wxGraphicsContext* gc, wxColour color,
        const std::vector<wxPoint2DDouble>& triangle);
    void drawDot(wxGraphicsContext* gc, double x, double y, std::string label);
    void strokeLine(wxGraphicsContext* gc, double x1, double y1, double x2, double y2);
    void strokeLines(wxGraphicsContext* gc, const std::vector<wxPoint2DDouble>& arr);
    std::vector<wxPoint2DDouble> getXformTriangle(int i);
    void highlightTriangle(wxGraphicsContext* gc, int i);
    bool pointInsideTriangle(wxPoint2DDouble p, int idx);
    double distancePointSegment(wxPoint2DDouble p, wxPoint2DDouble s1, wxPoint2DDouble s2);
    double sign(wxPoint2DDouble p1, wxPoint2DDouble p2, wxPoint2DDouble p3);
    Collision getCollision(wxPoint pos);
    CollisionType getCollisionType(wxPoint pos, int triangle);
    int checkVertexCollision(wxPoint p, int idx);
    int checkEdgeCollision(wxPoint p, int idx);
    void highlightVertex(wxGraphicsContext* gc, int triangle, int vertex);
    void highlightEdge(wxGraphicsContext* gc, int triangle, int edge);

    std::shared_ptr<core::Flame> flame;
    wxPanel* trianglePanel;
    int activeTransform;
    wxAffineMatrix2D affineTransform;
    wxPoint2DDouble center;
    wxColour gridColor, unitTriangleColor;
    double gridLowX, gridHighX;
    double gridLowY, gridHighY;
    double step;
    int zoomLevel;
    const double zoomFactor;
    static const int MAX_ZOOM_LEVEL = 400;
    bool draggingGrid;
    wxAffineMatrix2D dragInverseAffine;
    wxPoint2DDouble dragBegin, centerDragStart;
    std::vector<wxColour> xformColors;
    std::vector<std::string> dotLabels;
    int highlightedTriangle;
    bool draggingTriangle;
    wxPoint2DDouble originDragStart;
    CollisionType highlightType;
};

}