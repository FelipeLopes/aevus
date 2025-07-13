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
    aevusFrame->notifyActiveTransform(-1);
    aevusFrame->notifyFlameLoaded();
    sendTriangleContent();
    if (document->flameHasXForms()) {
        aevusFrame->notifyActiveTransform(0);
    }
}

void FlameView::sendTriangleContent() {
    XformTriangleContent content;
    content.triangles.resize(document->flame.xforms.size());
    if (content.triangles.empty()) {
        content.activeId = -1;
    } else {
        content.activeId = 0;
        for (int i=0; i<content.triangles.size(); i++) {
            auto triangle = document->flame.xforms.get(i)->coefs.value().triangle();
            content.triangles[i].o.x = triangle[0].first;
            content.triangles[i].o.y = triangle[0].second;
            content.triangles[i].x.x = triangle[1].first;
            content.triangles[i].x.y = triangle[1].second;
            content.triangles[i].y.x = triangle[2].first;
            content.triangles[i].y.y = triangle[2].second;
        }
    }
    triangleContentChanged(content);
}

}