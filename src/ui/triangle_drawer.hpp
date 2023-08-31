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
    bool setCursorCollision(Collision cursorCollision);
private:
    void drawInactiveTriangles(wxGraphicsContext* gc);
    void drawActiveTriangle(wxGraphicsContext* gc);
    void drawTriangleDots(wxGraphicsContext* gc, int idx);
    void drawDot(wxGraphicsContext* gc, GridPoint p, std::string label);
    void highlightTriangle(wxGraphicsContext* gc);
    void highlightVertex(wxGraphicsContext* gc);
    void highlightEdge(wxGraphicsContext* gc);
    std::vector<GridPoint> getXformTriangle(int i);
    wxColour getXformColor(int i);

    std::shared_ptr<core::Flame> flame;
    std::shared_ptr<TriangleGrid> triangleGrid;
    wxFont font;
    int activeTransform;
    std::vector<wxColour> xformColors;
    std::vector<std::string> dotLabels;
    Collision cursorCollision;
};

}