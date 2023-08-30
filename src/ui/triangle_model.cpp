#include "triangle_model.hpp"
#include "triangle_drawer.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"
#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>
#include <wx/affinematrix2dbase.h>
#include <wx/gdicmn.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

using std::vector;
using std::shared_ptr;
using std::string;
using core::Flame;

namespace ui {

TriangleModel::TriangleModel(shared_ptr<Flame> flame_, wxPanel* trianglePanel_):
    flame(flame_), trianglePanel(trianglePanel_), activeTransform(0), draggingGrid(false),
    xformColors({
        "#ff0000", "#cccc00", "#00cc00", "#00cccc", "#4040ff", "#cc00cc", "#cc8000",
        "#80004f", "#808022", "#608060", "#508080", "#4f4f80", "#805080", "#806022"
    }),
    dotLabels({"O", "X", "Y"}), highlightedTriangle(-1), draggingTriangle(false),
    highlightType(NO_COLLISION), draggingX(false), draggingY(false),
    rotatingTriangle(false), scalingTriangle(false)
{
    trianglePanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
    auto sz = trianglePanel->GetSize();
    double width = sz.GetWidth();
    double height = sz.GetHeight();
    triangleGrid = std::make_shared<TriangleGrid>(width, height);
    triangleCollider = std::make_shared<TriangleCollider>(flame, triangleGrid, activeTransform);
    triangleDrawer = std::make_shared<TriangleDrawer>(flame, triangleGrid,
        trianglePanel->GetFont(), activeTransform);
}

void TriangleModel::update() {
    trianglePanel->Refresh();
}

void TriangleModel::handleActiveXformChanged(int id) {
    activeTransform = id;
    triangleCollider->handleActiveXformChanged(id);
    update();
}

void TriangleModel::drawDot(wxGraphicsContext* gc, double x, double y, string label) {
    wxPoint2DDouble p(x, y);
    auto tp = triangleGrid->transformToWindow(p);
    gc->DrawEllipse(tp.m_x - 4, tp.m_y - 4, 8, 8);
    gc->DrawText(label, tp.m_x + 2, tp.m_y + 2);
}

void TriangleModel::drawTriangleDots(wxGraphicsContext* gc, wxColour color,
    const vector<GridPoint>& triangle)
{
    gc->SetPen(color);
    gc->SetBrush(*wxBLACK_BRUSH);
    auto panelFont = trianglePanel->GetFont();
    panelFont.SetPointSize(9);
    auto font = gc->CreateFont(panelFont, color);
    gc->SetFont(font);
    for (int j=0; j<3; j++) {
        auto p = triangle[j];
        drawDot(gc, p.m_x, p.m_y, dotLabels[j]);
    }
}

void TriangleModel::highlightVertex(wxGraphicsContext* gc, int triangle, int vertex) {
    auto tri = getXformTriangle(triangle);
    auto p = triangleGrid->transformToWindow(tri[vertex]);
    int numXformColors = xformColors.size();
    auto color = xformColors[triangle % numXformColors];
    gc->SetBrush(color);
    gc->SetPen(color);
    gc->DrawEllipse(p.m_x - 2, p.m_y - 2, 4, 4);
}

void TriangleModel::highlightEdge(wxGraphicsContext* gc, int triangle, int edge) {
    auto tri = getXformTriangle(triangle);
    auto s1 = triangleGrid->transformToWindow(tri[edge]);
    auto s2 = triangleGrid->transformToWindow(tri[edge+1]);
    int numXformColors = xformColors.size();
    auto c = xformColors[triangle % numXformColors];
    gc->SetPen(wxPen(wxColour(c.Red(), c.Green(), c.Blue(), 127), 5));
    gc->StrokeLine(s1.m_x, s1.m_y, s2.m_x, s2.m_y);
    drawTriangleDots(gc, c, tri);
}

void TriangleModel::drawXformTriangles(wxGraphicsContext* gc) {
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
    if (highlightedTriangle != -1) {
        highlightTriangle(gc, highlightedTriangle);
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

vector<GridPoint> TriangleModel::getXformTriangle(int i) {
    auto coefs = flame->xforms.get(i)->coefs.value();
    vector<GridPoint> triangle = {
        {coefs.ox, coefs.oy},
        {coefs.ox + coefs.xx, coefs.oy + coefs.xy},
        {coefs.ox + coefs.yx, coefs.oy + coefs.yy},
        {coefs.ox, coefs.oy}
    };
    return triangle;
}

void TriangleModel::highlightTriangle(wxGraphicsContext* gc, int i) {
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

void TriangleModel::handlePaint() {
    wxAutoBufferedPaintDC dc(trianglePanel);
    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        triangleGrid->drawGrid(gc);
        drawXformTriangles(gc);
    }
    delete gc;
}

void TriangleModel::handleResize(wxSizeEvent& event) {
    auto sz = trianglePanel->GetSize();
    triangleGrid->updateWindowSize(sz.GetWidth(), sz.GetHeight());
}

void TriangleModel::handleMouseWheel(wxMouseEvent &event) {
    if (event.GetWheelRotation() > 0) {
        triangleGrid->zoomIn();
    } else if (event.GetWheelRotation()) {
        triangleGrid->zoomOut();
    }
    trianglePanel->Refresh();
}

void TriangleModel::handleMouseUp(wxMouseEvent& event) {
    draggingGrid = false;
    draggingTriangle = false;
    draggingX = false;
    draggingY = false;
    rotatingTriangle = false;
    scalingTriangle = false;
}

void TriangleModel::handleMouseDown(wxMouseEvent& event) {
    auto pos = event.GetPosition();
    auto coll = triangleCollider->getCollision(WindowPoint(pos.x, pos.y));
    if (coll.type == NO_COLLISION) {
        draggingGrid = true;
        dragInverseAffine = triangleGrid->getAffineMatrix();
        dragInverseAffine.Invert();
        dragBegin = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
        centerDragStart = triangleGrid->getCenter();
    } else {
        if (coll.triangleId != activeTransform) {
            xformSelected(coll.triangleId);
        }
        switch (coll.type) {
            case VERTEX_O:
            case TRIANGLE_BODY: {
                draggingTriangle = true;
                dragInverseAffine = triangleGrid->getAffineMatrix();
                dragInverseAffine.Invert();
                dragBegin = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
                auto coefs = flame->xforms.get(activeTransform)->coefs.value();
                originDragStart = wxPoint2DDouble(coefs.ox, coefs.oy);
                break;
            }
            case VERTEX_X: {
                draggingX = true;
                dragInverseAffine = triangleGrid->getAffineMatrix();
                dragInverseAffine.Invert();
                dragBegin = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
                auto coefs = flame->xforms.get(activeTransform)->coefs.value();
                xDragStart = wxPoint2DDouble(coefs.xx, coefs.xy);
                break;
            }
            case VERTEX_Y: {
                draggingY = true;
                dragInverseAffine = triangleGrid->getAffineMatrix();
                dragInverseAffine.Invert();
                dragBegin = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
                auto coefs = flame->xforms.get(activeTransform)->coefs.value();
                yDragStart = wxPoint2DDouble(coefs.yx, coefs.yy);
                break;
            }
            case EDGE_OX:
            case EDGE_OY: {
                rotatingTriangle = true;
                dragInverseAffine = triangleGrid->getAffineMatrix();
                dragInverseAffine.Invert();
                dragBegin = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
                auto coefs = flame->xforms.get(activeTransform)->coefs.value();
                originDragStart = wxPoint2DDouble(coefs.ox, coefs.oy);
                xDragStart = wxPoint2DDouble(coefs.xx, coefs.xy);
                yDragStart = wxPoint2DDouble(coefs.yx, coefs.yy);
                break;
            }
            case EDGE_XY: {
                scalingTriangle = true;
                dragInverseAffine = triangleGrid->getAffineMatrix();
                dragInverseAffine.Invert();
                dragBegin = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
                auto coefs = flame->xforms.get(activeTransform)->coefs.value();
                originDragStart = wxPoint2DDouble(coefs.ox, coefs.oy);
                xDragStart = wxPoint2DDouble(coefs.xx, coefs.xy);
                yDragStart = wxPoint2DDouble(coefs.yx, coefs.yy);
                break;
            }
            default:
                break;
        }
    }
}

double TriangleModel::distancePointLine(wxPoint2DDouble p, wxPoint2DDouble s1, wxPoint2DDouble s2) {
    double d = s1.GetDistance(s2);
    if (d == 0) {
        return p.GetDistance(s1);
    } else {
        return (s1-p).GetCrossProduct(s2-p)/d;
    }
}

void TriangleModel::handleMouseMove(wxMouseEvent& event) {
    auto pos = event.GetPosition();
    if (draggingGrid) {
        pos.x = std::clamp(pos.x, 0, trianglePanel->GetSize().GetWidth());
        pos.y = std::clamp(pos.y, 0, trianglePanel->GetSize().GetHeight());
        auto dragEnd = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
        triangleGrid->updateCenter(centerDragStart + dragBegin - dragEnd);
        update();
    } else if (draggingTriangle) {
        pos.x = std::clamp(pos.x, 0, trianglePanel->GetSize().GetWidth());
        pos.y = std::clamp(pos.y, 0, trianglePanel->GetSize().GetHeight());
        auto dragEnd = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
        auto newOrigin = originDragStart + dragEnd - dragBegin;
        auto coefs = flame->xforms.get(activeTransform)->coefs.get();
        coefs->ox = newOrigin.m_x;
        coefs->oy = newOrigin.m_y;
        transformCoordsChanged();
    } else if (draggingX) {
        pos.x = std::clamp(pos.x, 0, trianglePanel->GetSize().GetWidth());
        pos.y = std::clamp(pos.y, 0, trianglePanel->GetSize().GetHeight());
        auto dragEnd = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
        auto newX = xDragStart + dragEnd - dragBegin;
        auto coefs = flame->xforms.get(activeTransform)->coefs.get();
        coefs->xx = newX.m_x;
        coefs->xy = newX.m_y;
        transformCoordsChanged();
    } else if (draggingY) {
        pos.x = std::clamp(pos.x, 0, trianglePanel->GetSize().GetWidth());
        pos.y = std::clamp(pos.y, 0, trianglePanel->GetSize().GetHeight());
        auto dragEnd = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
        auto newY = yDragStart + dragEnd - dragBegin;
        auto coefs = flame->xforms.get(activeTransform)->coefs.get();
        coefs->yx = newY.m_x;
        coefs->yy = newY.m_y;
        transformCoordsChanged();
    } else if (rotatingTriangle) {
        pos.x = std::clamp(pos.x, 0, trianglePanel->GetSize().GetWidth());
        pos.y = std::clamp(pos.y, 0, trianglePanel->GetSize().GetHeight());
        auto dragEnd = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
        double rot = (dragEnd-originDragStart).GetVectorAngle() -
            (dragBegin-originDragStart).GetVectorAngle();
        auto newX = xDragStart;
        newX.SetVectorAngle(xDragStart.GetVectorAngle()+rot);
        auto newY = yDragStart;
        newY.SetVectorAngle(yDragStart.GetVectorAngle()+rot);
        auto coefs = flame->xforms.get(activeTransform)->coefs.get();
        coefs->xx = newX.m_x;
        coefs->xy = newX.m_y;
        coefs->yx = newY.m_x;
        coefs->yy = newY.m_y;
        transformCoordsChanged();
    } else if (scalingTriangle) {
        pos.x = std::clamp(pos.x, 0, trianglePanel->GetSize().GetWidth());
        pos.y = std::clamp(pos.y, 0, trianglePanel->GetSize().GetHeight());
        auto dragEnd = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
        double sc = distancePointLine(dragEnd, originDragStart,
            originDragStart + xDragStart - yDragStart)/distancePointLine(dragBegin,
            originDragStart, originDragStart + xDragStart - yDragStart);
        auto newX = xDragStart;
        newX.SetVectorLength(xDragStart.GetVectorLength()*sc);
        auto newY = yDragStart;
        newY.SetVectorLength(yDragStart.GetVectorLength()*sc);
        auto coefs = flame->xforms.get(activeTransform)->coefs.get();
        coefs->xx = newX.m_x;
        coefs->xy = newX.m_y;
        coefs->yx = newY.m_x;
        coefs->yy = newY.m_y;
        transformCoordsChanged();
    } else {
        auto coll = triangleCollider->getCollision(WindowPoint(pos.x, pos.y));
        if (coll.triangleId != highlightedTriangle || coll.type != highlightType) {
            highlightedTriangle = coll.triangleId;
            highlightType = coll.type;
            update();
        }
    }
}

}