#include "triangle_model.hpp"
#include "triangle_grid.hpp"
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
}

void TriangleModel::update() {
    trianglePanel->Refresh();
}

void TriangleModel::handleActiveXformChanged(int id) {
    activeTransform = id;
    update();
}

void TriangleModel::drawDot(wxGraphicsContext* gc, double x, double y, string label) {
    wxPoint2DDouble p(x, y);
    auto tp = triangleGrid->getAffineMatrix().TransformPoint(p);
    gc->DrawEllipse(tp.m_x - 4, tp.m_y - 4, 8, 8);
    gc->DrawText(label, tp.m_x + 2, tp.m_y + 2);
}

void TriangleModel::drawTriangleDots(wxGraphicsContext* gc, wxColour color,
    const vector<wxPoint2DDouble>& triangle)
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
    auto p = triangleGrid->getAffineMatrix().TransformPoint(tri[vertex]);
    int numXformColors = xformColors.size();
    auto color = xformColors[triangle % numXformColors];
    gc->SetBrush(color);
    gc->SetPen(color);
    gc->DrawEllipse(p.m_x - 2, p.m_y - 2, 4, 4);
}

void TriangleModel::highlightEdge(wxGraphicsContext* gc, int triangle, int edge) {
    auto tri = getXformTriangle(triangle);
    auto s1 = triangleGrid->getAffineMatrix().TransformPoint(tri[edge]);
    auto s2 = triangleGrid->getAffineMatrix().TransformPoint(tri[edge+1]);
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
    vector<wxPoint2DDouble> fullPath = {
        {coefs.ox + coefs.xx, coefs.oy + coefs.xy},
        {coefs.ox, coefs.oy},
        {coefs.ox + coefs.yx, coefs.oy + coefs.yy}
    };
    vector<wxPoint2DDouble> dashedPath = {
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

bool TriangleModel::pointInsideTriangle(wxPoint2DDouble p, int idx) {
    auto v = getXformTriangle(idx);
    double d1 = sign(p, v[0], v[1]);
    double d2 = sign(p, v[1], v[2]);
    double d3 = sign(p, v[2], v[0]);
    bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(hasNeg && hasPos);
}

double TriangleModel::sign(wxPoint2DDouble p1, wxPoint2DDouble p2, wxPoint2DDouble p3) {
    return (p1.m_x-p3.m_x)*(p2.m_y-p3.m_y)-(p2.m_x-p3.m_x)*(p1.m_y-p3.m_y);
}

vector<wxPoint2DDouble> TriangleModel::getXformTriangle(int i) {
    auto coefs = flame->xforms.get(i)->coefs.value();
    vector<wxPoint2DDouble> triangle = {
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
        auto tp = triangleGrid->getAffineMatrix().TransformPoint(triangle[i]);
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
    auto coll = getCollision(pos);
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

int TriangleModel::checkVertexCollision(wxPoint p, int idx) {
    auto triangle = getXformTriangle(idx);
    // We check in order X -> Y -> O, because the X and Y vertices
    // are better when the triangle collapses to a point.
    for (int i=1; i<4; i++) {
        auto tp = triangleGrid->getAffineMatrix().TransformPoint(triangle[i]);
        if (tp.GetDistance(wxPoint2DDouble(p.x, p.y)) < 5) {
            return i;
        }
    }
    return -1;
}

double TriangleModel::distancePointSegment(wxPoint2DDouble p, wxPoint2DDouble s1,
    wxPoint2DDouble s2)
{
    double d2 = s1.GetDistanceSquare(s2);
    if (d2 == 0.0) {
        return p.GetDistance(s1);
    }
    double t = (p-s1).GetDotProduct(s2-s1)/d2;
    t = std::clamp(t, 0.0, 1.0);
    auto proj = s1 + t*(s2-s1);
    return p.GetDistance(proj);
}

double TriangleModel::distancePointLine(wxPoint2DDouble p, wxPoint2DDouble s1, wxPoint2DDouble s2) {
    double d = s1.GetDistance(s2);
    if (d == 0) {
        return p.GetDistance(s1);
    } else {
        return (s1-p).GetCrossProduct(s2-p)/d;
    }
}

int TriangleModel::checkEdgeCollision(wxPoint p, int idx) {
    auto triangle = getXformTriangle(idx);
    auto pc = wxPoint2DDouble(p.x, p.y);
    // We check XY last to avoid problems when the triangle
    // degenerates to a line.
    for (int i=0; i<3; i++) {
        auto s1 = triangleGrid->getAffineMatrix().TransformPoint(triangle[(i+2)%3]);
        auto s2 = triangleGrid->getAffineMatrix().TransformPoint(triangle[i%3]);
        if (distancePointSegment(pc, s1, s2) < 3) {
            return (i+2)%3;
        }
    }
    return -1;
}

TriangleModel::CollisionType TriangleModel::getCollisionType(wxPoint pos, int triangle) {
    auto ans = NO_COLLISION;
    int vertex = checkVertexCollision(pos, triangle);
    auto inverseAffine = triangleGrid->getAffineMatrix();
    inverseAffine.Invert();
    auto tp = inverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
    if (vertex != -1) {
        switch (vertex) {
            case 1: ans = VERTEX_X; break;
            case 2: ans = VERTEX_Y; break;
            case 3: ans = VERTEX_O; break;
        }
    } else {
        int edge = checkEdgeCollision(pos, triangle);
        if (edge != -1) {
            switch (edge) {
                case 0: ans = EDGE_OX; break;
                case 1: ans = EDGE_XY; break;
                case 2: ans = EDGE_OY; break;
            }
        } else if (pointInsideTriangle(tp, triangle)) {
            ans = TRIANGLE_BODY;
        }
    }
    return ans;
}

TriangleModel::Collision TriangleModel::getCollision(wxPoint pos) {
    TriangleModel::Collision ans;
    ans.triangleId = -1;
    ans.type = getCollisionType(pos, activeTransform);
    if (ans.type == NO_COLLISION) {
        for (int i=0; i<flame->xforms.size(); i++) {
            if (i == activeTransform) {
                continue;
            }
            ans.type = getCollisionType(pos, i);
            if (ans.type != NO_COLLISION) {
                ans.triangleId = i;
                break;
            }
        }
    } else {
        ans.triangleId = activeTransform;
    }
    return ans;
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
        auto coll = getCollision(pos);
        if (coll.triangleId != highlightedTriangle || coll.type != highlightType) {
            highlightedTriangle = coll.triangleId;
            highlightType = coll.type;
            update();
        }
    }
}

}