#include "flame_view.hpp"
#include "aevus_frame.hpp"
#include "content.hpp"
#include "flame_document.hpp"
#include "variation_model.hpp"
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
    // KLUDGE: wxWidgets document initialization is very weird.
    // First, it creates the document, then it creates the view,
    // attaches the view to the document, and only after attaching,
    // it creates the view.
    // The problem with this is that the document needs to notify
    // the view when it loads, since the LoadObject function is in
    // the document class. However, if the document is new, LoadObject
    // is never called, and when it's created, there's no way to get
    // the view since it wasn't created yet.

    // The way I solved this without generating document load events twice
    // is to save on initialization whether the document is new,
    // check if it is when the view is created, and then notify the UI.
    // In the case it's from a file, the notification will come from LoadObject,
    // when the data is available to the document.
    if (document->isNew()) {
        documentLoaded();
    }
    return true;
}

void FlameView::OnDraw(wxDC *dc) {
    // Stub function
}

bool FlameView::OnClose(bool deleteWindow) {
    aevusFrame->setupFlameView(NULL);
    flameContent(std::nullopt);
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
    activeXformId = document->flameHasXForms() ? 0 : -1;
    sendFlameContent();
    startNewRender();
}

void FlameView::handleXFormSelected(int i) {
    activeXformId = i;
    sendTriangleContent();
    sendPreTransformContent();
    sendPostTransformContent();
    sendWeightsContent();
    sendVariationContent();
    sendColorContent();
}

void FlameView::handleXFormAdded() {
    activeXformId = document->flame.xforms.size();
    document->flame.xforms.append(std::make_shared<core::XForm>());
    sendTriangleContent();
    sendPreTransformContent();
    sendPostTransformContent();
    sendWeightsContent();
    sendVariationContent();
    sendColorContent();
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
    sendVariationContent();
    sendColorContent();
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
    sendWeightsContent();
    startNewRender();
}

void FlameView::handleVariationAdded(core::Variation::VariationID id) {
    if (activeXformId == -1) {
        return;
    }
    auto varMap = &document->flame.xforms.get(activeXformId)->variationMap.get()->variations;
    if (varMap->find(id) != varMap->end()) {
        return;
    }
    (*varMap)[id] = core::VariationData(1.0, {});
    sendVariationContent();
    startNewRender();
}

void FlameView::handleVariationData(VariationDataParams params) {
    if (activeXformId == -1) {
        return;
    }
    auto varMap = &document->flame.xforms.get(activeXformId)->variationMap.get()->variations;
    if (varMap->find(params.id) == varMap->end()) {
        return;
    }
    if (params.data.weight == 0.0) {
        varMap->erase(params.id);
    } else {
        (*varMap)[params.id] = params.data;
    }
    sendVariationContent();
    startNewRender();
}

void FlameView::handleColorContent(ColorContent content) {
    if (activeXformId != -1) {
        document->flame.xforms.get(activeXformId)->color.setValue(content.color);
        document->flame.xforms.get(activeXformId)->colorSpeed.get()->colorSpeed = content.colorSpeed;
    }
    *(document->flame.palette.colors.get()) = content.palette;
    sendColorContent();
    startNewRender();
}

void FlameView::handleFrameContent(FrameContent content) {
    *(document->flame.size.get()) = content.flameSize;
    *(document->flame.center.get()) = content.flameCenter;
    document->flame.scale.setValue(content.flameScale);
    frameContent(content);
    startNewRender();
}

void FlameView::sendFlameContent() {
    FlameContent content;
    // Frame params
    content.frame.flameLoaded = true;
    content.frame.flameSize = document->flame.size.value();
    content.frame.flameCenter = document->flame.center.value();
    content.frame.flameScale = document->flame.scale.value();
    // Palette
    content.palette = document->flame.palette.colors.value();
    // XForms
    content.xforms.resize(document->flame.xforms.size());
    for (int i=0; i<content.xforms.size(); i++) {
        auto preCoefs = document->flame.xforms.get(i)->coefs.value();
        content.xforms[i].preCoefs.ox = preCoefs.ox;
        content.xforms[i].preCoefs.oy = preCoefs.oy;
        content.xforms[i].preCoefs.xx = preCoefs.xx;
        content.xforms[i].preCoefs.xy = preCoefs.xy;
        content.xforms[i].preCoefs.yx = preCoefs.yx;
        content.xforms[i].preCoefs.yy = preCoefs.yy;

        auto postCoefs = document->flame.xforms.get(i)->post.value();
        content.xforms[i].postCoefs.ox = postCoefs.ox;
        content.xforms[i].postCoefs.oy = postCoefs.oy;
        content.xforms[i].postCoefs.xx = postCoefs.xx;
        content.xforms[i].postCoefs.xy = postCoefs.xy;
        content.xforms[i].postCoefs.yx = postCoefs.yx;
        content.xforms[i].postCoefs.yy = postCoefs.yy;

        content.xforms[i].weight = document->flame.xforms.get(i)->weight.value();

        content.xforms[i].variations.variations = document->flame.xforms.get(i)->variationMap.get()->variations;

        content.xforms[i].color = document->flame.xforms.get(activeXformId)->color.value();
        content.xforms[i].colorSpeed = document->flame.xforms.get(activeXformId)->colorSpeed.value().colorSpeed;
    }
    // Final xform
    if (document->flame.finalXForm.isSet()) {
        content.finalXForm = XFormContent();
        auto finalPreCoefs = document->flame.finalXForm.get()->coefs.value();
        content.finalXForm->preCoefs.ox = finalPreCoefs.ox;
        content.finalXForm->preCoefs.oy = finalPreCoefs.oy;
        content.finalXForm->preCoefs.xx = finalPreCoefs.xx;
        content.finalXForm->preCoefs.xy = finalPreCoefs.xy;
        content.finalXForm->preCoefs.yx = finalPreCoefs.yx;
        content.finalXForm->preCoefs.yy = finalPreCoefs.yy;

        auto finalPostCoefs = document->flame.finalXForm.get()->post.value();
        content.finalXForm->postCoefs.ox = finalPostCoefs.ox;
        content.finalXForm->postCoefs.oy = finalPostCoefs.oy;
        content.finalXForm->postCoefs.xx = finalPostCoefs.xx;
        content.finalXForm->postCoefs.xy = finalPostCoefs.xy;
        content.finalXForm->postCoefs.yx = finalPostCoefs.yx;
        content.finalXForm->postCoefs.yy = finalPostCoefs.yy;

        content.finalXForm->color = document->flame.xforms.get(activeXformId)->color.value();
        content.finalXForm->colorSpeed = document->flame.xforms.get(activeXformId)->colorSpeed.value().colorSpeed;
    }
    flameContent(std::make_optional(content));
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

void FlameView::sendColorContent() {
    ColorContent content;
    content.flameLoaded = false;
    if (activeXformId != -1) {
        content.flameLoaded = true;
        content.color = document->flame.xforms.get(activeXformId)->color.value();
        content.colorSpeed = document->flame.xforms.get(activeXformId)->colorSpeed.value().colorSpeed;
    }
    content.palette = document->flame.palette.colors.value();
    colorContent(content);
}

void FlameView::sendFrameContent() {
    FrameContent content;
    content.flameLoaded = true;
    content.flameSize = document->flame.size.value();
    content.flameCenter = document->flame.center.value();
    content.flameScale = document->flame.scale.value();
    frameContent(content);
}

}