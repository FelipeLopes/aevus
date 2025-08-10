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
    sendActiveXFormContent();
}

void FlameView::handleXFormUpdate(ActiveXFormUpdateContent content) {
    if (content.preCoefs.has_value()) {
        auto ptr = document->flame.xforms.get(activeXformId)->coefs.get();
        ptr->ox = content.preCoefs->ox;
        ptr->oy = content.preCoefs->oy;
        ptr->xx = content.preCoefs->xx;
        ptr->xy = content.preCoefs->xy;
        ptr->yx = content.preCoefs->yx;
        ptr->yy = content.preCoefs->yy;
    }
    if (content.postCoefs.has_value()) {
        auto ptr = document->flame.xforms.get(activeXformId)->post.get();
        ptr->ox = content.postCoefs->ox;
        ptr->oy = content.postCoefs->oy;
        ptr->xx = content.postCoefs->xx;
        ptr->xy = content.postCoefs->xy;
        ptr->yx = content.postCoefs->yx;
        ptr->yy = content.postCoefs->yy;
    }
    if (content.weight.has_value()) {
        document->flame.xforms.get(activeXformId)->weight.setValue(content.weight.value());
    }
    if (content.variations.has_value()) {
        document->flame.xforms.get(activeXformId)->variationMap.get()->variations = content.variations.value();
    }
    if (content.color.has_value()) {
        document->flame.xforms.get(activeXformId)->color.setValue(content.color.value());
    }
    if (content.colorSpeed.has_value()) {
        document->flame.xforms.get(activeXformId)->colorSpeed.get()->colorSpeed = content.colorSpeed.value();
    }
    sendActiveXFormContent();
    startNewRender();
}

void FlameView::handleXFormAdded() {
    activeXformId = document->flame.xforms.size();
    document->flame.xforms.append(std::make_shared<core::XForm>());
    // TODO
    startNewRender();
}

void FlameView::handleXFormRemoved() {
    int sz = document->flame.xforms.size();
    document->flame.xforms.remove(activeXformId);
    if (activeXformId == sz - 1) {
        activeXformId--;
    }
    // TODO
    startNewRender();
}

void FlameView::handleFrameContent(FrameContent content) {
    *(document->flame.size.get()) = content.flameSize;
    *(document->flame.center.get()) = content.flameCenter;
    document->flame.scale.setValue(content.flameScale);
    // TODO
    startNewRender();
}

void FlameView::sendFlameContent() {
    FlameContent content;
    // Frame params
    content.frame.flameSize = document->flame.size.value();
    content.frame.flameCenter = document->flame.center.value();
    content.frame.flameScale = document->flame.scale.value();
    // Palette
    content.palette = document->flame.palette.colors.value();
    // XForms
    content.xforms.resize(document->flame.xforms.size());
    for (int i=0; i<content.xforms.size(); i++) {
        content.xforms[i] = getXformContent(i);
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

        content.finalXForm->color = document->flame.finalXForm.get()->color.value();
        content.finalXForm->colorSpeed = document->flame.finalXForm.get()->colorSpeed.value().colorSpeed;
    }
    flameContent(std::make_optional(content));
}

void FlameView::sendActiveXFormContent() {
    ActiveXFormContent content;
    content.id = activeXformId;
    content.xform = getXformContent(activeXformId);
    activeXformContent(content);
}

XFormContent FlameView::getXformContent(int idx) {
    XFormContent content;
    auto preCoefs = document->flame.xforms.get(idx)->coefs.value();
    content.preCoefs.ox = preCoefs.ox;
    content.preCoefs.oy = preCoefs.oy;
    content.preCoefs.xx = preCoefs.xx;
    content.preCoefs.xy = preCoefs.xy;
    content.preCoefs.yx = preCoefs.yx;
    content.preCoefs.yy = preCoefs.yy;

    auto postCoefs = document->flame.xforms.get(idx)->post.value();
    content.postCoefs.ox = postCoefs.ox;
    content.postCoefs.oy = postCoefs.oy;
    content.postCoefs.xx = postCoefs.xx;
    content.postCoefs.xy = postCoefs.xy;
    content.postCoefs.yx = postCoefs.yx;
    content.postCoefs.yy = postCoefs.yy;

    content.weight = document->flame.xforms.get(idx)->weight.value();

    content.variations = document->flame.xforms.get(idx)->variationMap.get()->variations;

    content.color = document->flame.xforms.get(idx)->color.value();
    content.colorSpeed = document->flame.xforms.get(idx)->colorSpeed.value().colorSpeed;
    return content;
}

}