#include "gradient_controller.hpp"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

namespace ui {

GradientController::GradientController(wxPanel* gradientPanel_):
    gradientPanel(gradientPanel_) { }

void GradientController::handlePaint() {
    wxAutoBufferedPaintDC dc(gradientPanel);
    dc.Clear();
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        delete gc;
    }
}

}