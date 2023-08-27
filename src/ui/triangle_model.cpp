#include "triangle_model.hpp"
#include <algorithm>
#include <iterator>
#include <vector>
#include <wx-3.2/wx/geometry.h>
#include <wx/affinematrix2dbase.h>
#include <wx/gdicmn.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

using std::map;
using std::vector;
using std::shared_ptr;
using core::Flame;

namespace ui {

TriangleModel::TriangleModel(shared_ptr<Flame> flame_, wxPanel* trianglePanel_):
    flame(flame_), trianglePanel(trianglePanel_), center(0,0), gridColor("#333333"),
    unitTriangleColor("#808080"), zoomLevel(0), zoomFactor(1.1), dragging(false)
{
    trianglePanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

double TriangleModel::calcStepForScale(double sc) {
    map<double, double> multiplier {
        {0.4, 1},
        {0.7, 0.5},
        {0.9, 0.2},
        {1.0, 0.1}
    };
    double logarithm = log10(sc);
    double periodBegin = floor(logarithm);
    double frac = logarithm - periodBegin;
    return pow(10, -periodBegin)*multiplier.lower_bound(frac)->second;
}

void TriangleModel::setupGrid() {
    wxMatrix2D identity;
    wxPoint2DDouble origin;
    affineTransform.Set(identity, origin);
    auto sz = trianglePanel->GetSize();
    double width = sz.GetWidth();
    double height = sz.GetHeight();
    affineTransform.Translate(width/2, height/2);
    double sc = pow(zoomFactor, zoomLevel);
    affineTransform.Scale(50*sc, -50*sc);
    step = calcStepForScale(sc);
    affineTransform.Translate(-center.m_x, -center.m_y);
    gridLowX = step*floor((-width/(2*sc) + center.m_x)/step);
    gridHighX = step*ceil((width/(2*sc) + center.m_x)/step);
    gridLowY = step*floor((-height/(2*sc) + center.m_y)/step);
    gridHighY = step*ceil((height/(2*sc) + center.m_y)/step);
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
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        gc->SetPen(gridColor);
        int nx = (gridHighX - gridLowX)/step;
        int ny = (gridHighY - gridLowY)/step;
        for (int i=0; i<=nx; i++) {
            double x = gridLowX + step*i;
            strokeLine(gc, x, gridLowY, x, gridHighY);
        }
        for (int i=0; i<=ny; i++) {
            double y = gridLowY + step*i;
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
    setupGrid();
}

void TriangleModel::handleMouseWheel(wxMouseEvent &event) {
    if (event.GetWheelRotation() > 0 && zoomLevel < MAX_ZOOM_LEVEL) {
        zoomLevel++;
    } else if (event.GetWheelRotation() < 0 && zoomLevel > -MAX_ZOOM_LEVEL) {
        zoomLevel--;
    }
    setupGrid();
    trianglePanel->Refresh();
}

void TriangleModel::handleMouseUp(wxMouseEvent& event) {
    dragging = false;
}

void TriangleModel::handleMouseDown(wxMouseEvent& event) {
    dragging = true;
    auto pos = event.GetPosition();
    dragInverseAffine = affineTransform;
    dragInverseAffine.Invert();
    dragBegin = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
    centerDragStart = center;
}

void TriangleModel::handleMouseMove(wxMouseEvent& event) {
    if (dragging) {
        auto pos = event.GetPosition();
        pos.x = std::clamp(pos.x, 0, trianglePanel->GetSize().GetWidth());
        pos.y = std::clamp(pos.y, 0, trianglePanel->GetSize().GetHeight());
        auto dragEnd = dragInverseAffine.TransformPoint(wxPoint2DDouble(pos.x, pos.y));
        center = centerDragStart + dragBegin - dragEnd;
        setupGrid();
        trianglePanel->Refresh();
    }
}

}