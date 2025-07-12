#include "flame_view.hpp"
#include "aevus_frame.hpp"
#include "flame_document.hpp"
#include <memory>
#include <wx/app.h>

namespace ui {

wxIMPLEMENT_DYNAMIC_CLASS(FlameView, wxView);

FlameView::FlameView() {
    aevusFrame = dynamic_cast<AevusFrame*>(wxTheApp->GetTopWindow());
    SetFrame(aevusFrame);
}

bool FlameView::OnCreate(wxDocument *doc, long flags) {
    if (!wxView::OnCreate(doc, flags)){
        return false;
    }
    document = dynamic_cast<FlameDocument*>(doc);
    // KLUDGE: although it looks strange to instantiate the flame
    // here, instead of inside the document class,
    // it's the best option we have, given the weird order
    // of initialization in wxWidgets views and documents.
    document->flame = new core::Flame;
    aevusFrame->setupFlameView(this);
    documentLoaded();
    return true;
}

void FlameView::OnDraw(wxDC *dc) {
    // Stub function
}

bool FlameView::OnClose(bool deleteWindow) {
    delete document->flame;
    aevusFrame->setupFlameView(NULL);
    return wxView::OnClose(deleteWindow);
}

void FlameView::OnChangeFilename() {
    // Stub function
}

core::Flame* FlameView::getFlame() const
{
    return document->flame;
}

void FlameView::documentLoaded() {
    if (document->flameHasXForms()) {
        aevusFrame->notifyFlameLoaded();
        aevusFrame->notifyActiveTransform(0);
    } else {
        aevusFrame->notifyActiveTransform(-1);
        aevusFrame->notifyFlameLoaded();
    }
}

}