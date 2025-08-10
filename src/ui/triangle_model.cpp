#include "triangle_model.hpp"
#include "content.hpp"
#include "triangle_collider.hpp"
#include "triangle_drawer.hpp"
#include "triangle_grid.hpp"
#include "triangle_types.hpp"
#include <wx/dcbuffer.h>

using std::array;
using std::pair;
using std::string;

namespace ui {

TriangleModel::TriangleModel(wxPanel* trianglePanel_):
    trianglePanel(trianglePanel_),
    activeId(-1),
    triangleGrid(trianglePanel->GetSize()),
    triangleCollider(&triangleGrid),
    triangleDrawer(&triangleGrid, trianglePanel->GetFont()),
    triangleUpdater(&triangleGrid)
{
    trianglePanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

array<pair<double, double>, 3> TriangleModel::triangle(CoefsContent c) {
    return {{
        {c.ox, c.oy},
        {c.ox + c.xx, c.oy + c.xy},
        {c.ox + c.yx, c.oy + c.yy}
    }};
}

void TriangleModel::update() {
    trianglePanel->Refresh();
}

void TriangleModel::handlePaint() {
    wxAutoBufferedPaintDC dc(trianglePanel);
    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        triangleGrid.drawGrid(gc);
        triangleDrawer.drawXformTriangles(gc);
    }
    delete gc;
}

void TriangleModel::handleResize(wxSizeEvent& event) {
    auto sz = trianglePanel->GetSize();
    triangleGrid.updateWindowSize(sz.GetWidth(), sz.GetHeight());
}

void TriangleModel::handleMouseWheel(wxMouseEvent &event) {
    if (event.GetWheelRotation() > 0) {
        triangleGrid.zoomIn();
    } else if (event.GetWheelRotation() < 0) {
        triangleGrid.zoomOut();
    }
    trianglePanel->Refresh();
}

void TriangleModel::handleMouseUp(wxMouseEvent& event) {
    triangleUpdater.finishUpdate();
}

void TriangleModel::handleMouseDown(wxMouseEvent& event) {
    auto pos = event.GetPosition();
    WindowPoint pt(pos.x, pos.y);
    auto coll = triangleCollider.getCollision(pt);
    if (coll.type == NO_COLLISION) {
        triangleUpdater.startGridDrag(pt);
    } else {
        if (coll.triangleId != activeId) {
            xformSelected(coll.triangleId);
        }
        switch (coll.type) {
            case VERTEX_O:
            case TRIANGLE_BODY:
                triangleUpdater.startTriangleDrag(pt); break;
            case VERTEX_X:
                triangleUpdater.startXDrag(pt); break;
            case VERTEX_Y:
                triangleUpdater.startYDrag(pt); break;
            case EDGE_OX:
            case EDGE_OY:
                triangleUpdater.startTriangleRotation(pt); break;
            case EDGE_XY:
                triangleUpdater.startTriangleScaling(pt); break;
            default: break;
        }
    }
}

void TriangleModel::handleMouseMove(wxMouseEvent& event) {
    auto pos = event.GetPosition();
    auto state = triangleUpdater.getUpdateState();
    if (state != NO_UPDATE) {
        pos.x = std::clamp(pos.x, 0, trianglePanel->GetSize().GetWidth());
        pos.y = std::clamp(pos.y, 0, trianglePanel->GetSize().GetHeight());
        WindowPoint pt(pos.x, pos.y);
        if (state == DRAGGING_GRID) {
            triangleUpdater.updateCenterForPoint(pt);
            update();
        } else {
            auto coefs = triangleUpdater.getCoefsForPoint(pt);
            xformCoefsChanged(coefs);
        }
    } else {
        auto coll = triangleCollider.getCollision(WindowPoint(pos.x, pos.y));
        if (triangleDrawer.setCursorCollision(coll)) {
            update();
        }
    }
}

void TriangleModel::handleFlameContent(std::optional<FlameContent> content_) {
    if (!content_.has_value()) {
        activeId = -1;
        triangleCollider.handleContent(std::nullopt);
        triangleDrawer.handleContent(std::nullopt);
        triangleUpdater.handleContent(std::nullopt);
    } else {
        auto content = content_.value();
        auto sz = content.xforms.size();
        activeId = (sz == 0) ? -1 : 0;
        XFormTriangleContent triangleContent;
        triangleContent.activeId = activeId;
        triangleContent.coefs.resize(sz);
        for (int i=0; i<sz; i++) {
            triangleContent.coefs[i] = content.xforms[i].preCoefs;
        }
        triangleCollider.handleContent(triangleContent);
        triangleDrawer.handleContent(triangleContent);
        triangleUpdater.handleContent(triangleContent);
    }
    update();
}

void TriangleModel::handleActiveXformContent(ActiveXFormContent xformContent) {
    activeId = xformContent.id;
    triangleCollider.handleActiveXformContent(xformContent);
    triangleDrawer.handleActiveXformContent(xformContent);
    triangleUpdater.handleActiveXformContent(xformContent);
    update();
}

}