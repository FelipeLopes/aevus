#pragma once

#include <memory>
#include "../core/flame.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"

namespace ui {

class TriangleDrawer {
public:
    TriangleDrawer(std::shared_ptr<core::Flame> flame, std::shared_ptr<TriangleGrid> triangleGrid,
        wxFont font, int activeTransform);
    void handleActiveXformChanged(int id);
    void drawXformTriangles(wxGraphicsContext* gc);
    void setCursorCollision(Collision cursorCollision);
private:
    void drawTriangleDots(wxGraphicsContext* gc, wxColour color,
        const std::vector<GridPoint>& triangle);
    void drawDot(wxGraphicsContext* gc, double x, double y, std::string label);
    void highlightTriangle(wxGraphicsContext* gc, int i);
    void highlightVertex(wxGraphicsContext* gc, int triangle, int vertex);
    void highlightEdge(wxGraphicsContext* gc, int triangle, int edge);
    std::vector<GridPoint> getXformTriangle(int i);

    std::shared_ptr<core::Flame> flame;
    std::shared_ptr<TriangleGrid> triangleGrid;
    wxFont font;
    int activeTransform;
    std::vector<wxColour> xformColors;
    std::vector<std::string> dotLabels;
    Collision cursorCollision;
};

}