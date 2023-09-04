#include "triangle_model.hpp"
#include "triangle_drawer.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"
#include <wx/dcbuffer.h>

using std::make_shared;
using std::string;
using core::Flame;

namespace ui {

TriangleModel::TriangleModel(Flame* flame_, wxPanel* trianglePanel_):
    flame(flame_), trianglePanel(trianglePanel_), activeTransform(-1)
{
    trianglePanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
    auto sz = trianglePanel->GetSize();
    double width = sz.GetWidth();
    double height = sz.GetHeight();
    triangleGrid = make_shared<TriangleGrid>(width, height);
    triangleCollider = make_shared<TriangleCollider>(flame, triangleGrid, activeTransform);
    triangleDrawer = make_shared<TriangleDrawer>(flame, triangleGrid,
        trianglePanel->GetFont(), activeTransform);
    triangleUpdater = make_shared<TriangleUpdater>(flame, triangleGrid, activeTransform);
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
}

void TriangleModel::handleMouseDown(wxMouseEvent& event) {
    auto pos = event.GetPosition();
    WindowPoint pt(pos.x, pos.y);
    auto coll = triangleCollider->getCollision(pt);
    if (coll.type == NO_COLLISION) {
        triangleUpdater->startGridDrag(pt);
    } else {
        if (coll.triangleId != activeTransform) {
            xformSelected(coll.triangleId);
        }
        switch (coll.type) {
            case VERTEX_O:
            case TRIANGLE_BODY:
                triangleUpdater->startTriangleDrag(pt); break;
            case VERTEX_X:
                triangleUpdater->startXDrag(pt); break;
            case VERTEX_Y:
                triangleUpdater->startYDrag(pt); break;
            case EDGE_OX:
            case EDGE_OY:
                triangleUpdater->startTriangleRotation(pt); break;
            case EDGE_XY:
                triangleUpdater->startTriangleScaling(pt); break;
            default: break;
        }
    }
}

void TriangleModel::handleMouseMove(wxMouseEvent& event) {
    auto pos = event.GetPosition();
    auto state = triangleUpdater->getUpdateState();
    if (state != NO_UPDATE) {
        pos.x = std::clamp(pos.x, 0, trianglePanel->GetSize().GetWidth());
        pos.y = std::clamp(pos.y, 0, trianglePanel->GetSize().GetHeight());
        WindowPoint pt(pos.x, pos.y);
        triangleUpdater->setUpdatePoint(pt);
        if (state == DRAGGING_GRID) {
            update();
        } else {
            transformCoordsChanged();
        }
    } else {
        auto coll = triangleCollider->getCollision(WindowPoint(pos.x, pos.y));
        if (triangleDrawer->setCursorCollision(coll)) {
            update();
        }
    }
}

}