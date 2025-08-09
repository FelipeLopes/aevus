#pragma once

#include "../core/flame.hpp"
#include "content.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"

namespace ui {

class TriangleDrawer {
public:
    TriangleDrawer(TriangleGrid* triangleGrid, wxFont font);
    void handleContent(std::optional<XFormTriangleContent> content);
    void handleNewActiveId(int id);
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

    TriangleGrid* triangleGrid;
    wxFont font;
    std::optional<XFormTriangleContent> content;
    std::vector<wxColour> xformColors;
    std::vector<std::string> dotLabels;
    Collision cursorCollision;
};

}