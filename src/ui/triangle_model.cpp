#include "triangle_model.hpp"
#include <algorithm>
#include <iterator>
#include <vector>
#include <wx-3.2/wx/geometry.h>
#include <wx/affinematrix2dbase.h>
#include <wx/gdicmn.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

using std::vector;
using std::shared_ptr;
using core::Flame;

namespace ui {

TriangleModel::TriangleModel(shared_ptr<Flame> flame_, wxPanel* trianglePanel_):
    flame(flame_), trianglePanel(trianglePanel_), gridColor("#333333"),
    unitTriangleColor("#808080"), zoomLevel(0), zoomFactor(1.1), dragging(false)
{
    trianglePanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void TriangleModel::setupAffine() {
    wxMatrix2D identity;
    wxPoint2DDouble origin;
    affineTransform.Set(identity, origin);
    auto sz = trianglePanel->GetSize();
    double width = sz.GetWidth();
    double height = sz.GetHeight();
    affineTransform.Translate(width/2, height/2);
    double sc = 50*pow(zoomFactor, zoomLevel);
    affineTransform.Scale(sc, -sc);
    gridLowX = floor(-width/(2*sc));
    gridHighX = ceil(width/(2*sc));
    gridLowY = floor(-height/(2*sc));
    gridHighY = ceil(height/(2*sc));
}

void TriangleModel::strokeLine(wxGraphicsContext* gc, double x1, double y1, double x2, double y2) {
    // We implement our own strokeLine wrapper that accesses the affine
    // transform directly instead of using the builtin wxWidgets constructions.
    // The reason for this is that when stroking a line, the minimum pen width is 1,
    // and wxWidgets scales this width in the final custom control, making their
    // builtin methods useless for our purposes.
    wxPoint2DDouble p1(x1, y1), p2(x2, y2);
    auto tp1 = affineTransform.TransformPoint(p1);
    auto tp2 = affineTransform.TransformPoint(p2);
    gc->StrokeLine(tp1.m_x, tp1.m_y, tp2.m_x, tp2.m_y);
}

void TriangleModel::strokeLines(wxGraphicsContext* gc, const vector<wxPoint2DDouble>& arr) {
    vector<wxPoint2DDouble> transformedArr;
    std::transform(arr.begin(), arr.end(), std::back_inserter(transformedArr),
        [this](wxPoint2DDouble pt) {
            return affineTransform.TransformPoint(pt);
        });
    gc->StrokeLines(transformedArr.size(), transformedArr.data());
}

void TriangleModel::handlePaint() {
    wxAutoBufferedPaintDC dc(trianglePanel);
    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();
    setupAffine();
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        gc->SetPen(gridColor);
        int nx = gridHighX - gridLowX;
        int ny = gridHighY - gridLowY;
        for (int i=0; i<=nx; i++) {
            double x = gridLowX + i;
            strokeLine(gc, x, gridLowY, x, gridHighY);
        }
        for (int i=0; i<=ny; i++) {
            double y = gridLowY + i;
            strokeLine(gc, gridLowX, y, gridHighX, y);
        }
        gc->SetPen(wxPen(unitTriangleColor, 1, wxPENSTYLE_SHORT_DASH));
        vector<wxPoint2DDouble> unitTriangle = {
            {0, 0}, {1, 0}, {0, 1}, {0, 0}
        };
        strokeLines(gc, unitTriangle);
    }
    delete gc;
}

void TriangleModel::handleResize(wxSizeEvent& event) {
    trianglePanel->Refresh();
}

void TriangleModel::handleMouseWheel(wxMouseEvent &event) {
    if (event.GetWheelRotation() > 0 && zoomLevel < MAX_ZOOM_LEVEL) {
        zoomLevel++;
    } else if (event.GetWheelRotation() < 0 && zoomLevel > -MAX_ZOOM_LEVEL) {
        zoomLevel--;
    }
    trianglePanel->Refresh();
}

void TriangleModel::handleMouseUp(wxMouseEvent& event) {
    dragging = false;
}

void TriangleModel::handleMouseDown(wxMouseEvent& event) {
    dragging = true;
}

void TriangleModel::handleMouseMove(wxMouseEvent& event) {
    if (dragging) {
        auto cursorPos = event.GetPosition();
        cursorPos.x = std::clamp(cursorPos.x, 0, trianglePanel->GetSize().GetWidth());
        cursorPos.y = std::clamp(cursorPos.y, 0, trianglePanel->GetSize().GetHeight());
        printf("%d %d\n", cursorPos.x, cursorPos.y);
    }
}

}