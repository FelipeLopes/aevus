#pragma once

#include <memory>
#include "../core/flame.hpp"
#include <wx-3.2/wx/geometry.h>
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
private:
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
    double sign(wxPoint2DDouble p1, wxPoint2DDouble p2, wxPoint2DDouble p3);

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
    bool dragging;
    wxAffineMatrix2D dragInverseAffine;
    wxPoint2DDouble dragBegin, centerDragStart;
    std::vector<wxColour> xformColors;
    std::vector<std::string> dotLabels;
    int highlightedTriangle;
};

}