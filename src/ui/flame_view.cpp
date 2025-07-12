#include "flame_view.hpp"
#include "aevus_frame.hpp"
#include <memory>
#include <wx/app.h>

namespace ui {

wxIMPLEMENT_DYNAMIC_CLASS(FlameView, wxView);

FlameView::FlameView() {
    SetFrame(wxTheApp->GetTopWindow());
}

void FlameView::updateFrame() {
    dynamic_cast<AevusFrame*>(GetFrame())->setupForFlameView(this);
}

bool FlameView::OnCreate(wxDocument *doc, long flags) {
    if (!wxView::OnCreate(doc, flags)){
        return false;
    }
    flame = new core::Flame;
    updateFrame();
    return true;
}

void FlameView::OnDraw(wxDC *dc) {
    // Stub function
}

bool FlameView::OnClose(bool deleteWindow) {
    delete flame;
    flame = NULL;
    updateFrame();
    return wxView::OnClose(deleteWindow);
}

void FlameView::OnChangeFilename() {
    // Stub function
}

FlameDocument* FlameView::getDocument() const
{
    return wxStaticCast(wxView::GetDocument(), FlameDocument);
}

}