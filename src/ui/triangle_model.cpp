#include "triangle_model.hpp"
#include <wx/affinematrix2d.h>
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
        auto sz = trianglePanel->GetSize();
        wxAffineMatrix2D transform;
        transform.Translate((sz.GetWidth()+0.0)/2, (sz.GetHeight()+0.0)/2);
        transform.Scale(50, -50);
        gc->SetTransform(gc->CreateMatrix(transform));
        gc->SetBrush(*wxWHITE_BRUSH);
        gc->DrawRectangle(0, 0, 1, 1);
    }
    delete gc;
}

}