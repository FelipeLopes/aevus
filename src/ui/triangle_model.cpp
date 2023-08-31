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

using std::shared_ptr;
using std::string;
using core::Flame;

namespace ui {

TriangleModel::TriangleModel(shared_ptr<Flame> flame_, wxPanel* trianglePanel_):
    flame(flame_), trianglePanel(trianglePanel_), activeTransform(0),
    draggingX(false), draggingY(false), rotatingTriangle(false), scalingTriangle(false)
{
    trianglePanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
    auto sz = trianglePanel->GetSize();
    double width = sz.GetWidth();
    double height = sz.GetHeight();
    triangleGrid = std::make_shared<TriangleGrid>(width, height);
    triangleCollider = std::make_shared<TriangleCollider>(flame, triangleGrid, activeTransform);
    triangleDrawer = std::make_shared<TriangleDrawer>(flame, triangleGrid,
        trianglePanel->GetFont(), activeTransform);
    triangleUpdater = std::make_shared<TriangleUpdater>(flame, triangleGrid, activeTransform);
}

void TriangleModel::update() {
    trianglePanel->Refresh();
}

void TriangleModel::handleActiveXformChanged(int id) {
    activeTransform = id;
    triangleCollider->handleActiveXformChanged(id);
    triangleDrawer->handleActiveXformChanged(id);
    triangleUpdater->handleActiveXformChanged(id);
    update();
}

void TriangleModel::handlePaint() {
    wxAutoBufferedPaintDC dc(trianglePanel);
    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        triangleGrid->drawGrid(gc);
        triangleDrawer->drawXformTriangles(gc);
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
    triangleUpdater->finishUpdate();
    draggingX = false;
    draggingY = false;
    rotatingTriangle = false;
    scalingTriangle = false;
}

void TriangleModel::handleMouseDown(wxMouseEvent& event) {
    auto pos = event.GetPosition();
    auto coll = triangleCollider->getCollision(WindowPoint(pos.x, pos.y));
    if (coll.type == NO_COLLISION) {
        triangleUpdater->startGridDrag(WindowPoint(pos.x, pos.y));
    } else {
        if (coll.triangleId != activeTransform) {
            xformSelected(coll.triangleId);
        }
        switch (coll.type) {
            case VERTEX_O:
            case TRIANGLE_BODY: {
                triangleUpdater->startTriangleDrag(WindowPoint(pos.x, pos.y));
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
    switch (triangleUpdater->getUpdateState()) {
        case DRAGGING_GRID: {
            pos.x = std::clamp(pos.x, 0, trianglePanel->GetSize().GetWidth());
            pos.y = std::clamp(pos.y, 0, trianglePanel->GetSize().GetHeight());
            triangleUpdater->setUpdatePoint(WindowPoint(pos.x, pos.y));
            update();
            break;
        }
        case DRAGGING_TRIANGLE: {
            pos.x = std::clamp(pos.x, 0, trianglePanel->GetSize().GetWidth());
            pos.y = std::clamp(pos.y, 0, trianglePanel->GetSize().GetHeight());
            triangleUpdater->setUpdatePoint(WindowPoint(pos.x, pos.y));
            transformCoordsChanged();
            break;
        }
        default: break;
    }
    if (draggingX) {
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
        if (triangleDrawer->setCursorCollision(coll)) {
            update();
        }
    }
}

}