#include "triangle_model.hpp"
#include <wx/affinematrix2dbase.h>
#include <wx/geometry.h>
#include <wx/gdicmn.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

using std::shared_ptr;
using core::Flame;

namespace ui {

TriangleModel::TriangleModel(shared_ptr<Flame> flame_, wxPanel* trianglePanel_):
    flame(flame_), trianglePanel(trianglePanel_)
{
    trianglePanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void TriangleModel::handlePaint() {
    wxAutoBufferedPaintDC dc(trianglePanel);
    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        gc->SetTransform(gc->CreateMatrix(affineTransform));
        gc->SetBrush(*wxWHITE_BRUSH);
        gc->DrawRectangle(0, 0, 1, 1);
    }
    delete gc;
}

void TriangleModel::handleResize(wxSizeEvent& event) {
    wxMatrix2D identity;
    wxPoint2DDouble origin;
    affineTransform.Set(identity, origin);
    auto sz = event.GetSize();
    affineTransform.Translate(sz.GetWidth()/2.0, sz.GetHeight()/2.0);
    affineTransform.Scale(50, -50);
}

}