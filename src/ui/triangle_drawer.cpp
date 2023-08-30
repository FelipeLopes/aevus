#include "triangle_drawer.hpp"
#include <wx/brush.h>
#include <wx/pen.h>

using core::Flame;
using std::shared_ptr;
using std::string;
using std::vector;

namespace ui {

TriangleDrawer::TriangleDrawer(shared_ptr<Flame> flame_, shared_ptr<TriangleGrid> triangleGrid_,
    wxFont font_, int activeTransform_): flame(flame_), triangleGrid(triangleGrid_),
    font(font_), activeTransform(activeTransform_), xformColors({
        "#ff0000", "#cccc00", "#00cc00", "#00cccc", "#4040ff", "#cc00cc", "#cc8000",
        "#80004f", "#808022", "#608060", "#508080", "#4f4f80", "#805080", "#806022"
    }), dotLabels({"O", "X", "Y"}) { }

void TriangleDrawer::handleActiveXformChanged(int id) {
    activeTransform = id;
}

void TriangleDrawer::drawXformTriangles(wxGraphicsContext* gc) {
    int sz = flame->xforms.size();
    int numXformColors = xformColors.size();
    for (int i=0; i<sz; i++) {
        if (i == activeTransform) {
            continue;
        }
        auto triangle = getXformTriangle(i);
        auto color = xformColors[i%numXformColors];
        gc->SetPen(wxPen(color, 1, wxPENSTYLE_SHORT_DASH));
        triangleGrid->strokeLines(gc, triangle);
        drawTriangleDots(gc, color, triangle);
    }
    auto color = xformColors[activeTransform%numXformColors];
    auto coefs = flame->xforms.get(activeTransform)->coefs.value();
    auto triangle = getXformTriangle(activeTransform);
    vector<GridPoint> fullPath = {
        {coefs.ox + coefs.xx, coefs.oy + coefs.xy},
        {coefs.ox, coefs.oy},
        {coefs.ox + coefs.yx, coefs.oy + coefs.yy}
    };
    vector<GridPoint> dashedPath = {
        {coefs.ox + coefs.xx, coefs.oy + coefs.xy},
        {coefs.ox + coefs.yx, coefs.oy + coefs.yy}
    };
    gc->SetPen(wxPen(color, 1, wxPENSTYLE_SHORT_DASH));
    triangleGrid->strokeLines(gc, dashedPath);
    gc->SetPen(color);
    triangleGrid->strokeLines(gc, fullPath);
    drawTriangleDots(gc, color, triangle);
    int highlightedTriangle = cursorCollision.triangleId;
    if (highlightedTriangle != -1) {
        highlightTriangle(gc, highlightedTriangle);
        int highlightType = cursorCollision.type;
        switch (highlightType) {
            case VERTEX_O: highlightVertex(gc, highlightedTriangle, 0); break;
            case VERTEX_X: highlightVertex(gc, highlightedTriangle, 1); break;
            case VERTEX_Y: highlightVertex(gc, highlightedTriangle, 2); break;
            case EDGE_OX: highlightEdge(gc, highlightedTriangle, 0); break;
            case EDGE_OY: highlightEdge(gc, highlightedTriangle, 2); break;
            case EDGE_XY: highlightEdge(gc, highlightedTriangle, 1); break;
            default: break;
        }
    }
}

void TriangleDrawer::setCursorCollision(Collision cursorCollision_) {
    cursorCollision = cursorCollision_;
}

void TriangleDrawer::drawTriangleDots(wxGraphicsContext* gc, wxColour color,
    const vector<GridPoint>& triangle)
{
    gc->SetPen(color);
    gc->SetBrush(*wxBLACK_BRUSH);
    font.SetPointSize(9);
    auto gcFont = gc->CreateFont(font, color);
    gc->SetFont(gcFont);
    for (int j=0; j<3; j++) {
        auto p = triangle[j];
        drawDot(gc, p.m_x, p.m_y, dotLabels[j]);
    }
}

void TriangleDrawer::drawDot(wxGraphicsContext* gc, double x, double y, string label) {
    wxPoint2DDouble p(x, y);
    auto tp = triangleGrid->transformToWindow(p);
    gc->DrawEllipse(tp.m_x - 4, tp.m_y - 4, 8, 8);
    gc->DrawText(label, tp.m_x + 2, tp.m_y + 2);
}

void TriangleDrawer::highlightTriangle(wxGraphicsContext* gc, int i) {
    auto triangle = getXformTriangle(i);
    int numXformColors = xformColors.size();
    auto color = xformColors[i % numXformColors];
    auto path = gc->CreatePath();
    for (int i=0; i<3; i++) {
        auto tp = triangleGrid->transformToWindow(triangle[i]);
        path.AddLineToPoint(tp);
    }
    gc->SetBrush(wxColour(color.Red(), color.Green(), color.Blue(), 127));
    gc->FillPath(path);
    drawTriangleDots(gc, color, triangle);
}

void TriangleDrawer::highlightVertex(wxGraphicsContext* gc, int triangle, int vertex) {
    auto tri = getXformTriangle(triangle);
    auto p = triangleGrid->transformToWindow(tri[vertex]);
    int numXformColors = xformColors.size();
    auto color = xformColors[triangle % numXformColors];
    gc->SetBrush(color);
    gc->SetPen(color);
    gc->DrawEllipse(p.m_x - 2, p.m_y - 2, 4, 4);
}

void TriangleDrawer::highlightEdge(wxGraphicsContext* gc, int triangle, int edge) {
    auto tri = getXformTriangle(triangle);
    auto s1 = triangleGrid->transformToWindow(tri[edge]);
    auto s2 = triangleGrid->transformToWindow(tri[edge+1]);
    int numXformColors = xformColors.size();
    auto c = xformColors[triangle % numXformColors];
    gc->SetPen(wxPen(wxColour(c.Red(), c.Green(), c.Blue(), 127), 5));
    gc->StrokeLine(s1.m_x, s1.m_y, s2.m_x, s2.m_y);
    drawTriangleDots(gc, c, tri);
}

vector<GridPoint> TriangleDrawer::getXformTriangle(int i) {
    auto coefs = flame->xforms.get(i)->coefs.value();
    vector<GridPoint> triangle = {
        {coefs.ox, coefs.oy},
        {coefs.ox + coefs.xx, coefs.oy + coefs.xy},
        {coefs.ox + coefs.yx, coefs.oy + coefs.yy},
        {coefs.ox, coefs.oy}
    };
    return triangle;
}

}