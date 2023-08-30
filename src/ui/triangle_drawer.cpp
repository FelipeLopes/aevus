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
    }), numXformColors(xformColors.size()), dotLabels({"O", "X", "Y"}) { }

void TriangleDrawer::handleActiveXformChanged(int id) {
    activeTransform = id;
}

void TriangleDrawer::drawXformTriangles(wxGraphicsContext* gc) {
    drawInactiveTriangles(gc);
    drawActiveTriangle(gc);
    highlightTriangle(gc);
    highlightEdge(gc);
    drawTriangleDots(gc);
    highlightVertex(gc);
}

bool TriangleDrawer::setCursorCollision(Collision cursorCollision_) {
    if (cursorCollision.triangleId != cursorCollision_.triangleId ||
        cursorCollision.type != cursorCollision_.type)
    {
        cursorCollision = cursorCollision_;
        return true;
    }
    return false;
}

void TriangleDrawer::drawInactiveTriangles(wxGraphicsContext* gc) {
    int sz = flame->xforms.size();
    for (int i=0; i<sz; i++) {
        if (i != activeTransform) {
            auto triangle = getXformTriangle(i);
            auto color = xformColors[i % numXformColors];
            gc->SetPen(wxPen(color, 1, wxPENSTYLE_SHORT_DASH));
            triangleGrid->strokeLines(gc, triangle);
        }
    }
}

void TriangleDrawer::drawActiveTriangle(wxGraphicsContext* gc) {
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
}

void TriangleDrawer::drawTriangleDots(wxGraphicsContext* gc) {
    int sz = flame->xforms.size();
    for (int i=0; i<sz; i++) {
        auto color = xformColors[i % numXformColors];
        auto triangle = getXformTriangle(i);
        gc->SetPen(color);
        gc->SetBrush(*wxBLACK_BRUSH);
        font.SetPointSize(9);
        auto gcFont = gc->CreateFont(font, color);
        gc->SetFont(gcFont);
        for (int i=0; i<3; i++) {
            auto p = triangle[i];
            drawDot(gc, p.m_x, p.m_y, dotLabels[i]);
        }
    }
}

void TriangleDrawer::drawDot(wxGraphicsContext* gc, double x, double y, string label) {
    wxPoint2DDouble p(x, y);
    auto tp = triangleGrid->transformToWindow(p);
    gc->DrawEllipse(tp.m_x - 4, tp.m_y - 4, 8, 8);
    gc->DrawText(label, tp.m_x + 2, tp.m_y + 2);
}

void TriangleDrawer::highlightTriangle(wxGraphicsContext* gc) {
    int idx = cursorCollision.triangleId;
    if (idx != -1) {
        auto triangle = getXformTriangle(idx);
        auto color = xformColors[idx % numXformColors];
        auto path = gc->CreatePath();
        for (int i=0; i<3; i++) {
            auto tp = triangleGrid->transformToWindow(triangle[i]);
            path.AddLineToPoint(tp);
        }
        gc->SetBrush(wxColour(color.Red(), color.Green(), color.Blue(), 127));
        gc->FillPath(path);
    }
}

void TriangleDrawer::highlightVertex(wxGraphicsContext* gc) {
    int idx = cursorCollision.triangleId;
    if (idx != -1) {
        int vertex = -1;
        switch(cursorCollision.type) {
            case VERTEX_O: vertex = 0; break;
            case VERTEX_X: vertex = 1; break;
            case VERTEX_Y: vertex = 2; break;
            default: break;
        }
        if (vertex != -1) {
            auto tri = getXformTriangle(idx);
            auto p = triangleGrid->transformToWindow(tri[vertex]);
            auto color = xformColors[idx % numXformColors];
            gc->SetBrush(color);
            gc->SetPen(color);
            gc->DrawEllipse(p.m_x - 2, p.m_y - 2, 4, 4);
        }
    }
}

void TriangleDrawer::highlightEdge(wxGraphicsContext* gc) {
    int idx = cursorCollision.triangleId;
    if (idx != -1) {
        int edge = -1;
        switch (cursorCollision.type) {
            case EDGE_OX: edge = 0; break;
            case EDGE_OY: edge = 2; break;
            case EDGE_XY: edge = 1; break;
            default: break;
        }
        if (edge != -1) {
            auto tri = getXformTriangle(idx);
            auto s1 = triangleGrid->transformToWindow(tri[edge]);
            auto s2 = triangleGrid->transformToWindow(tri[edge+1]);
            auto c = xformColors[idx % numXformColors];
            gc->SetPen(wxPen(wxColour(c.Red(), c.Green(), c.Blue(), 127), 5));
            gc->StrokeLine(s1.m_x, s1.m_y, s2.m_x, s2.m_y);
        }
    }
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