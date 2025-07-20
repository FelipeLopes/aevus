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
        sendPreTransformContent();
        sendPostTransformContent();
    } else {
        noTransformContent();
    }
    sendTriangleContent();
    sendWeightsContent();
    sendVariationContent();
    startNewRender();
}

void FlameView::handleXFormSelected(int i) {
    activeXformId = i;
    sendTriangleContent();
    sendPreTransformContent();
    sendPostTransformContent();
    sendWeightsContent();
}

void FlameView::handleXFormAdded() {
    activeXformId = document->flame.xforms.size();
    document->flame.xforms.append(std::make_shared<core::XForm>());
    sendTriangleContent();
    sendPreTransformContent();
    sendPostTransformContent();
    sendWeightsContent();
    startNewRender();
}

void FlameView::handleXFormRemoved() {
    int sz = document->flame.xforms.size();
    document->flame.xforms.remove(activeXformId);
    if (activeXformId == sz - 1) {
        activeXformId--;
    }
    sendTriangleContent();
    sendPreTransformContent();
    sendPostTransformContent();
    sendWeightsContent();
    startNewRender();
}

void FlameView::handleTriangleCoefs(CoefsContent coefs) {
    auto ptr = document->flame.xforms.get(activeXformId)->coefs.get();
    ptr->ox = coefs.ox;
    ptr->oy = coefs.oy;
    ptr->xx = coefs.xx;
    ptr->xy = coefs.xy;
    ptr->yx = coefs.yx;
    ptr->yy = coefs.yy;
    sendTriangleContent();
    sendPreTransformContent();
    sendPostTransformContent();
    startNewRender();
}

void FlameView::handleCoefsPreListCtrl(CoefsContent coefs) {
    handleTriangleCoefs(coefs);
}

void FlameView::handleCoefsPostListCtrl(CoefsContent coefs) {
    auto ptr = document->flame.xforms.get(activeXformId)->post.get();
    ptr->ox = coefs.ox;
    ptr->oy = coefs.oy;
    ptr->xx = coefs.xx;
    ptr->xy = coefs.xy;
    ptr->yx = coefs.yx;
    ptr->yy = coefs.yy;
    sendTriangleContent();
    sendPreTransformContent();
    sendPostTransformContent();
    startNewRender();
}

void FlameView::handleWeights(WeightsContent content) {
    int sz = content.weights.size();
    for (int i=0; i<sz; i++) {
        document->flame.xforms.get(i)->weight.setValue(content.weights[i]);
    }
    startNewRender();
}

void FlameView::sendTriangleContent() {
    XFormTriangleContent content;
    content.coefs.resize(document->flame.xforms.size());
    content.activeId = activeXformId;
    for (int i=0; i<content.coefs.size(); i++) {
        auto flameCoefs = document->flame.xforms.get(i)->coefs.value();

        CoefsContent messageCoefs;
        messageCoefs.ox = flameCoefs.ox;
        messageCoefs.oy = flameCoefs.oy;
        messageCoefs.xx = flameCoefs.xx;
        messageCoefs.xy = flameCoefs.xy;
        messageCoefs.yx = flameCoefs.yx;
        messageCoefs.yy = flameCoefs.yy;

        content.coefs[i] = messageCoefs;
    }
    triangleContentChanged(content);
}

void FlameView::sendPreTransformContent() {
    if (activeXformId == -1) {
        noTransformContent();
        return;
    }
    auto vals = document->flame.xforms.get(activeXformId)->coefs.value();

    CoefsContent messageCoefs;
    messageCoefs.ox = vals.ox;
    messageCoefs.oy = vals.oy;
    messageCoefs.xx = vals.xx;
    messageCoefs.xy = vals.xy;
    messageCoefs.yx = vals.yx;
    messageCoefs.yy = vals.yy;

    preTransformContent(messageCoefs);
}

void FlameView::sendPostTransformContent() {
    if (activeXformId == -1) {
        noTransformContent();
        return;
    }
    auto vals = document->flame.xforms.get(activeXformId)->post.value();

    CoefsContent messageCoefs;
    messageCoefs.ox = vals.ox;
    messageCoefs.oy = vals.oy;
    messageCoefs.xx = vals.xx;
    messageCoefs.xy = vals.xy;
    messageCoefs.yx = vals.yx;
    messageCoefs.yy = vals.yy;

    postTransformContent(messageCoefs);
}

void FlameView::sendWeightsContent() {
    WeightsContent content;
    content.flameLoaded = true;
    content.activeId = activeXformId;
    content.weights.resize(document->flame.xforms.size());
    for (int i=0; i<content.weights.size(); i++) {
        content.weights[i] = document->flame.xforms.get(i)->weight.value();
    }
    weightsContent(content);
}

void FlameView::sendVariationContent() {
    VariationContent content;
    if (activeXformId != -1) {
        content.variations = document->flame.xforms.get(activeXformId)->variationMap.get()->variations;
    }
    variationContent(content);
}

}