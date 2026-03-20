#include "flame_view.hpp"
#include "aevus_frame.hpp"
#include "commands.hpp"
#include "flame_document.hpp"
#include <cstddef>
#include <wx/app.h>

using core::ActiveXFormContent;
using core::ActiveXFormUpdateContent;
using core::FlameContent;
using core::XFormContent;
using core::FrameContent;

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
    // Empty, the document is drawn in separate widgets
}

bool FlameView::OnClose(bool deleteWindow) {
    flameContent(std::nullopt);
    aevusFrame->setupFlameView(NULL);
    return wxView::OnClose(deleteWindow);
}

// WARNING: this function gets called not only when the filename is changed,
// but every time the document is modified, so the name is a bit misleading
void FlameView::OnChangeFilename() {
    wxString appName = wxTheApp->GetAppDisplayName();
    wxString title;

    wxString docName = GetDocument()->GetUserReadableName();
    title = appName + wxString(_(" - ")) + docName + (GetDocument()->IsModified() ? "*" : "");

    GetFrame()->SetLabel(title);
}

core::FlameV* FlameView::getFlame() const
{
    return &document->flame;
}

void FlameView::documentLoaded() {
    activeXformId = document->flameHasXForms() ? 0 : -1;
    sendFlameContent();
}

void FlameView::handleXFormSelected(int i) {
    activeXformId = i;
    sendSelectedXFormContent();
}

void FlameView::setActiveXFormId(int id) {
    int sz = document->flame.xforms.size();
    if (id < 0) {
        activeXformId = 0;
    } else if (id >= sz) {
        activeXformId = sz;
    } else {
        activeXformId = id;
    }
}

void FlameView::handleXFormUpdate(ActiveXFormUpdateContent content) {
    auto oldXForm = document->flame.xforms[activeXformId];
    auto newXForm = oldXForm;
    if (content.preCoefs.has_value()) {
        auto ptr = &newXForm.coefs;
        ptr->ox = content.preCoefs->ox;
        ptr->oy = content.preCoefs->oy;
        ptr->xx = content.preCoefs->xx;
        ptr->xy = content.preCoefs->xy;
        ptr->yx = content.preCoefs->yx;
        ptr->yy = content.preCoefs->yy;
    }
    if (content.postCoefs.has_value()) {
        auto ptr = &newXForm.post;
        ptr->ox = content.postCoefs->ox;
        ptr->oy = content.postCoefs->oy;
        ptr->xx = content.postCoefs->xx;
        ptr->xy = content.postCoefs->xy;
        ptr->yx = content.postCoefs->yx;
        ptr->yy = content.postCoefs->yy;
    }
    if (content.weight.has_value()) {
        newXForm.weight = content.weight.value();
    }
    if (content.variations.has_value()) {
        newXForm.variationMap.variations = content.variations.value();
    }
    if (content.color.has_value()) {
        newXForm.color = content.color.value();
    }
    if (content.colorSpeed.has_value()) {
        newXForm.colorSpeed = content.colorSpeed.value();
    }
    if (xFormBeforeExplore.has_value()) {
        document->flame.xforms[activeXformId] = newXForm;
        sendUpdatedXFormContent();
    } else {
        document->GetCommandProcessor()->Submit(new XFormUpdateCommand(this, activeXformId, oldXForm, newXForm));
    }
    document->Modify(true);
}

void FlameView::handleXFormAdded(int id) {
    document->GetCommandProcessor()->Submit(new XFormAddCommand(this, activeXformId));
    document->Modify(true);
}

void FlameView::handleXFormRemoved(int id) {
    auto oldXForm = document->flame.xforms[activeXformId];
    document->GetCommandProcessor()->Submit(new XFormRemoveCommand(this, activeXformId, oldXForm));
    document->Modify(true);
}

void FlameView::handleFrameContent(FrameContent content) {
    FrameContent oldContent;
    oldContent.flameSize = document->flame.size;
    oldContent.flameCenter = document->flame.center;
    oldContent.flameScale = document->flame.scale;
    document->GetCommandProcessor()->Submit(new FrameUpdateCommand(this, oldContent, content));
    document->Modify(true);
}

void FlameView::sendFlameContent() {
    FlameContent content;
    // Frame params
    content.frame.flameSize = document->flame.size;
    content.frame.flameCenter = document->flame.center;
    content.frame.flameScale = document->flame.scale;
    // Render params
    content.render.quality = document->flame.quality;
    content.render.brightness = document->flame.brightness;
    content.render.contrast = document->flame.contrast;
    content.render.gamma = document->flame.gamma;
    content.render.vibrancy = document->flame.vibrancy;
    content.render.background = document->flame.background;
    content.render.clipping = document->flame.clippingMode;
    // Palette
    content.palette = document->flame.palette.colors;
    // XForms
    content.xforms.resize(document->flame.xforms.size());
    for (int i=0; i<content.xforms.size(); i++) {
        content.xforms[i] = getXformContent(i).value();
    }
    // Final xform
    if (document->flame.finalXForm.has_value()) {
        content.finalXForm = XFormContent();
        auto finalPreCoefs = document->flame.finalXForm->coefs;
        content.finalXForm->preCoefs.ox = finalPreCoefs.ox;
        content.finalXForm->preCoefs.oy = finalPreCoefs.oy;
        content.finalXForm->preCoefs.xx = finalPreCoefs.xx;
        content.finalXForm->preCoefs.xy = finalPreCoefs.xy;
        content.finalXForm->preCoefs.yx = finalPreCoefs.yx;
        content.finalXForm->preCoefs.yy = finalPreCoefs.yy;

        auto finalPostCoefs = document->flame.finalXForm->post;
        content.finalXForm->postCoefs.ox = finalPostCoefs.ox;
        content.finalXForm->postCoefs.oy = finalPostCoefs.oy;
        content.finalXForm->postCoefs.xx = finalPostCoefs.xx;
        content.finalXForm->postCoefs.xy = finalPostCoefs.xy;
        content.finalXForm->postCoefs.yx = finalPostCoefs.yx;
        content.finalXForm->postCoefs.yy = finalPostCoefs.yy;

        content.finalXForm->variations = document->flame.finalXForm->variationMap.variations;

        content.finalXForm->color = document->flame.finalXForm->color;
        content.finalXForm->colorSpeed = document->flame.finalXForm->colorSpeed;
    }
    flameContent(std::make_optional(content));
}

void FlameView::sendFrameContent() {
    FrameContent content;
    content.flameSize = document->flame.size;
    content.flameCenter = document->flame.center;
    content.flameScale = document->flame.scale;
    frameContent(content);
}

void FlameView::sendSelectedXFormContent() {
    ActiveXFormContent content;
    content.id = activeXformId;
    content.op = core::SELECTED;
    content.xform = getXformContent(activeXformId);
    activeXformContent(content);
}

void FlameView::sendUpdatedXFormContent() {
    ActiveXFormContent content;
    content.id = activeXformId;
    content.op = core::UPDATED;
    content.xform = getXformContent(activeXformId);
    activeXformContent(content);
}

void FlameView::sendAddedXFormContent() {
    ActiveXFormContent content;
    content.id = activeXformId;
    content.op = core::ADDED;
    content.xform = getXformContent(activeXformId);
    activeXformContent(content);
}

void FlameView::sendRemovedXFormContent(int id) {
    ActiveXFormContent content;
    content.id = id;
    content.op = core::REMOVED;
    content.xform = getXformContent(activeXformId);
    activeXformContent(content);
}

std::optional<XFormContent> FlameView::getXformContent(int idx) {
    if (idx < 0 || idx >= document->flame.xforms.size()) {
        return std::nullopt;
    }
    XFormContent content;
    auto preCoefs = document->flame.xforms[idx].coefs;
    content.preCoefs.ox = preCoefs.ox;
    content.preCoefs.oy = preCoefs.oy;
    content.preCoefs.xx = preCoefs.xx;
    content.preCoefs.xy = preCoefs.xy;
    content.preCoefs.yx = preCoefs.yx;
    content.preCoefs.yy = preCoefs.yy;

    auto postCoefs = document->flame.xforms[idx].post;
    content.postCoefs.ox = postCoefs.ox;
    content.postCoefs.oy = postCoefs.oy;
    content.postCoefs.xx = postCoefs.xx;
    content.postCoefs.xy = postCoefs.xy;
    content.postCoefs.yx = postCoefs.yx;
    content.postCoefs.yy = postCoefs.yy;

    content.weight = document->flame.xforms[idx].weight;

    content.variations = document->flame.xforms[idx].variationMap.variations;

    content.color = document->flame.xforms[idx].color;
    content.colorSpeed = document->flame.xforms[idx].colorSpeed;
    return content;
}

void FlameView::handleStartXFormExplore() {
    xFormBeforeExplore = document->flame.xforms[activeXformId];
}

void FlameView::handleStopXFormExplore() {
    core::XFormV newXForm = document->flame.xforms[activeXformId];
    document->GetCommandProcessor()->Submit(new XFormUpdateCommand(this, activeXformId, xFormBeforeExplore.value(), newXForm));
    xFormBeforeExplore.reset();
    document->Modify(true);
}

}