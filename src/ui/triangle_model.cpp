#include "triangle_model.hpp"
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
}

}