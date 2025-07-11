#include "flame_view.hpp"
#include "aevus_frame.hpp"
#include <memory>
#include <wx/app.h>

namespace ui {

wxIMPLEMENT_DYNAMIC_CLASS(FlameView, wxView);

FlameView::FlameView() {
    SetFrame(wxTheApp->GetTopWindow());
}

bool FlameView::OnCreate(wxDocument *doc, long flags) {
    if (!wxView::OnCreate(doc, flags)){
        return false;
    }
    flame = new core::Flame;
    dynamic_cast<AevusFrame*>(GetFrame())->setupForFlameView(this);
    return true;
}

void FlameView::OnDraw(wxDC *dc) {
    // Stub function
}

bool FlameView::OnClose(bool deleteWindow) {
    delete flame;
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