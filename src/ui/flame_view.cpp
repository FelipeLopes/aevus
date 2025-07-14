#include "flame_view.hpp"
#include "aevus_frame.hpp"
#include "content.hpp"
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
    aevusFrame->setupFlameView(this);
    documentLoaded();
    return true;
}

void FlameView::OnDraw(wxDC *dc) {
    // Stub function
}

bool FlameView::OnClose(bool deleteWindow) {
    aevusFrame->setupFlameView(NULL);
    aevusFrame->notifyActiveTransform(-1);
    return wxView::OnClose(deleteWindow);
}

void FlameView::OnChangeFilename() {
    // Stub function
}

core::Flame* FlameView::getFlame() const
{
    return &document->flame;
}

void FlameView::documentLoaded() {
    activeXformId = -1;
    aevusFrame->notifyActiveTransform(-1);
    aevusFrame->notifyFlameLoaded();
    if (document->flameHasXForms()) {
        activeXformId = 0;
        aevusFrame->notifyActiveTransform(0);
    }
    sendTriangleContent();
}

void FlameView::handleXFormSelected(int i) {
    activeXformId = i;
    sendTriangleContent();
}

void FlameView::handleTriangleCoefs(CoefsContent coefs) {
    printf("called flameview\n");
}

void FlameView::sendTriangleContent() {
    XFormTriangleContent content;
    content.triangles.resize(document->flame.xforms.size());
    content.activeId = activeXformId;
    for (int i=0; i<content.triangles.size(); i++) {
        XFormTriangle triangle;
        triangle.arr = document->flame.xforms.get(i)->coefs.value().triangle();
        triangle.coefs = document->flame.xforms.get(i)->coefs.value();
        content.triangles[i]=triangle;
    }
    triangleContentChanged(content);
}

}