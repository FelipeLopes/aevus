#include "flame_model.hpp"
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

namespace ui {

FlameModel::FlameModel(wxPanel* flamePanel_): flamePanel(flamePanel_) {
    flamePanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void FlameModel::handlePaint() {
    wxAutoBufferedPaintDC dc(flamePanel);
    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        if (flameBitmap.IsOk()) {
            gc->DrawBitmap(flameBitmap, 0, 0, flameBitmap.GetWidth(), flameBitmap.GetHeight());
        }
        delete gc;
    }
}

void FlameModel::setBitmap(const wxBitmap &bitmap) {
    flameBitmap = bitmap;
    flamePanel->Refresh();
}

}