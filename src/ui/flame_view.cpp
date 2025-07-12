#include "flame_view.hpp"
#include "aevus_frame.hpp"
#include "flame_document.hpp"
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
    this->document = dynamic_cast<FlameDocument*>(doc);
    dynamic_cast<AevusFrame*>(GetFrame())->setupForFlameView(this);
    return true;
}

void FlameView::OnDraw(wxDC *dc) {
    // Stub function
}

bool FlameView::OnClose(bool deleteWindow) {
    dynamic_cast<AevusFrame*>(GetFrame())->setupForFlameView(NULL);
    return wxView::OnClose(deleteWindow);
}

void FlameView::OnChangeFilename() {
    // Stub function
}

core::Flame* FlameView::getFlame() const
{
    return &document->flame;
}

}