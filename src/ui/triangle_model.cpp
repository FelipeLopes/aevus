#include "triangle_model.hpp"
#include <wx/affinematrix2dbase.h>
#include <wx/gdicmn.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

using std::shared_ptr;
using core::Flame;

namespace ui {

TriangleModel::TriangleModel(shared_ptr<Flame> flame_, wxPanel* trianglePanel_):
    flame(flame_), trianglePanel(trianglePanel_), gridColor("#333333"),
    unitTriangleColor("#808080")
{
    trianglePanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
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

void TriangleModel::handlePaint() {
    wxAutoBufferedPaintDC dc(trianglePanel);
    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();
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
    }
    delete gc;
}

void TriangleModel::handleResize(wxSizeEvent& event) {
    wxMatrix2D identity;
    wxPoint2DDouble origin;
    affineTransform.Set(identity, origin);
    auto sz = event.GetSize();
    double width = sz.GetWidth();
    double height = sz.GetHeight();
    affineTransform.Translate(width/2, height/2);
    affineTransform.Scale(50, -50);
    gridLowX = floor(-width/100);
    gridHighX = ceil(width/100);
    gridLowY = floor(-height/100);
    gridHighY = ceil(height/100);
}

}