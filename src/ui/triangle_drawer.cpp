#include "triangle_drawer.hpp"
#include "content.hpp"
#include "triangle_model.hpp"
#include "triangle_types.hpp"
#include <wx/brush.h>
#include <wx/pen.h>

using std::string;
using std::vector;

namespace ui {

TriangleDrawer::TriangleDrawer(TriangleGrid* triangleGrid_, wxFont font_):
    triangleGrid(triangleGrid_), font(font_), xformColors({
        "#ff0000", "#cccc00", "#00cc00", "#00cccc", "#4040ff", "#cc00cc", "#cc8000",
        "#80004f", "#808022", "#608060", "#508080", "#4f4f80", "#805080", "#806022"
    }), dotLabels({"O", "X", "Y"})
{
    content->activeId = -1;
}

void TriangleDrawer::drawXformTriangles(wxGraphicsContext* gc) {
    if (!content.has_value() || content->activeId == -1) {
        return;
    }
    drawInactiveTriangles(gc);
    drawActiveTriangle(gc);
    highlightTriangle(gc);
    highlightEdge(gc);
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
    int sz = content->coefs.size();
    for (int i=0; i<sz; i++) {
        if (i != content->activeId) {
            auto triangle = getXformTriangle(i);
            triangle.push_back(triangle[0]);
            auto color = getXformColor(i);
            gc->SetPen(wxPen(color, 1, wxPENSTYLE_SHORT_DASH));
            triangleGrid->strokeLines(gc, triangle);
            drawTriangleDots(gc, i);
        }
    }
}

void TriangleDrawer::drawActiveTriangle(wxGraphicsContext* gc) {
    auto color = getXformColor(content->activeId);
    auto path = getXformTriangle(content->activeId);
    auto ori = path[0];
    path.erase(path.begin());
    gc->SetPen(wxPen(color, 1, wxPENSTYLE_SHORT_DASH));
    triangleGrid->strokeLines(gc, path);
    path.insert(path.begin()+1, ori);
    gc->SetPen(color);
    triangleGrid->strokeLines(gc, path);
    drawTriangleDots(gc, content->activeId);
}

void TriangleDrawer::drawTriangleDots(wxGraphicsContext* gc, int idx) {
    auto color = getXformColor(idx);
    auto triangle = getXformTriangle(idx);
    gc->SetPen(color);
    gc->SetBrush(*wxBLACK_BRUSH);
    font.SetPointSize(9);
    auto gcFont = gc->CreateFont(font, color);
    gc->SetFont(gcFont);
    for (int i=0; i<3; i++) {
        drawDot(gc, triangle[i], dotLabels[i]);
    }
}

void TriangleDrawer::drawDot(wxGraphicsContext* gc, GridPoint p, string label) {
    auto tp = triangleGrid->transformToWindow(p);
    gc->DrawEllipse(tp.m_x - 4, tp.m_y - 4, 8, 8);
    gc->DrawText(label, tp.m_x + 2, tp.m_y + 2);
}

void TriangleDrawer::highlightTriangle(wxGraphicsContext* gc) {
    int idx = cursorCollision.triangleId;
    if (idx != -1) {
        auto triangle = getXformTriangle(idx);
        auto color = getXformColor(idx);
        auto path = gc->CreatePath();
        for (int i=0; i<3; i++) {
            auto tp = triangleGrid->transformToWindow(triangle[i]);
            path.AddLineToPoint(tp);
        }
        gc->SetBrush(wxColour(color.Red(), color.Green(), color.Blue(), 127));
        gc->FillPath(path);
        drawTriangleDots(gc, idx);
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
            auto color = getXformColor(idx);
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
            auto c = getXformColor(idx);
            gc->SetPen(wxPen(wxColour(c.Red(), c.Green(), c.Blue(), 127), 5));
            triangleGrid->strokeLine(gc, tri[edge], tri[(edge+1)%3]);
            drawTriangleDots(gc, idx);
        }
    }
}

vector<GridPoint> TriangleDrawer::getXformTriangle(int i) {
    vector<GridPoint> ans;
    auto arr = TriangleModel::triangle(content->coefs[i]);
    std::transform(arr.begin(), arr.end(), std::back_inserter(ans),
        [](auto pair) {
            return GridPoint(pair);
        });
    return ans;
}

wxColour TriangleDrawer::getXformColor(int i) {
    return xformColors[i % xformColors.size()];
}

}