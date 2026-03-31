#include "aevus_frame.hpp"
#include <memory>
#include <wx/gdicmn.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/mstream.h>
#include <wx/wfstream.h>
#include "flame_model.hpp"
#include "flame_view.hpp"
#include "transform_model.hpp"
#include "triangle_model.hpp"
#include "wxfb_frame.h"

using namespace boost::signals2;
using boost::bind;
using clwrap::OpenCL;
using render::Renderer;
using std::optional;
using std::string;

namespace ui {

AevusFrame::AevusFrame(wxDocManager* manager, OpenCL* openCL, optional<string> filename):
    WxfbFrame(manager, NULL),
    docManager(manager),
    context(openCL->createQueuedContext(0, 1)),
    renderer(context, flameStream),
    flameModel(flameWindow, flameStream),
    preTransformModel(preTransformDataViewCtrl, resetPreButton, true),
    postTransformModel(postTransformDataViewCtrl, resetPostButton, false),
    weightsModel(weightsDataViewCtrl, addXformButton, removeXformButton),
    variationModel(variationListCtrl, variationTextCtrl),
    colorModel(colorListCtrl, palettePanel),
    frameModel(frameListCtrl),
    triangleModel(trianglePanel)
{
    renderer.imageRendered.connect(bind(&FlameModel::update, &flameModel));

    addXformButton->SetBitmap(loadEmbeddedPNG(
        _binary_res_plus_default_png_start,
        _binary_res_plus_default_png_end
    ));
    addXformButton->SetBitmapSelected(loadEmbeddedPNG(
        _binary_res_plus_selected_png_start,
        _binary_res_plus_selected_png_end
    ));
    removeXformButton->SetBitmap(loadEmbeddedPNG(
        _binary_res_minus_default_png_start,
        _binary_res_minus_default_png_end
    ));
    removeXformButton->SetBitmapSelected(loadEmbeddedPNG(
        _binary_res_minus_selected_png_start,
        _binary_res_minus_selected_png_end
    ));

    trianglePanel->SetFocus();

    if (filename.has_value()) {
        grad::Grd5Stream grd5Stream(filename->c_str());
        auto gradList = grd5Stream.readGradientList();
        for (auto g: gradList.gradients) {
            if (auto solidGrad = std::dynamic_pointer_cast<grad::Grd5SolidGradient>(g)) {
                presetLibrary.addGrd5Gradient(*solidGrad);
            }
        }
    }
}

void AevusFrame::setupFlameView(FlameView *flameView_) {
    for (int i=0; i<connections.size(); i++) {
        connections[i].disconnect();
    }
    connections.clear();
    flameView = flameView_;
    if (paletteFrame != NULL) {
        paletteFrame->setupFlameView(flameView);
    }
    if (flameView != NULL) {
        connections.push_back(flameView->flameContent.connect(
            bind(&Renderer::handleFlameContent, &renderer, _1)
        ));
        connections.push_back(flameView->flameContent.connect(
            bind(&TransformModel::handleFlameContent, &preTransformModel, _1)
        ));
        connections.push_back(flameView->flameContent.connect(
            bind(&TransformModel::handleFlameContent, &postTransformModel, _1)
        ));
        connections.push_back(flameView->flameContent.connect(
            bind(&TriangleModel::handleFlameContent, &triangleModel, _1)
        ));
        connections.push_back(flameView->flameContent.connect(
            bind(&WeightsModel::handleFlameContent, &weightsModel, _1)
        ));
        connections.push_back(flameView->flameContent.connect(
            bind(&VariationModel::handleFlameContent, &variationModel, _1)
        ));
        connections.push_back(flameView->flameContent.connect(
            bind(&ColorModel::handleFlameContent, &colorModel, _1)
        ));
        connections.push_back(flameView->flameContent.connect(
            bind(&FrameModel::handleFlameContent, &frameModel, _1)
        ));
        connections.push_back(flameView->activeXformContent.connect(
            bind(&Renderer::handleActiveXformContent, &renderer, _1)
        ));
        connections.push_back(flameView->activeXformContent.connect(
            bind(&TransformModel::handleActiveXformContent, &preTransformModel, _1)
        ));
        connections.push_back(flameView->activeXformContent.connect(
            bind(&TransformModel::handleActiveXformContent, &postTransformModel, _1)
        ));
        connections.push_back(flameView->activeXformContent.connect(
            bind(&TriangleModel::handleActiveXformContent, &triangleModel, _1)
        ));
        connections.push_back(flameView->activeXformContent.connect(
            bind(&VariationModel::handleActiveXformContent, &variationModel, _1)
        ));
        connections.push_back(flameView->activeXformContent.connect(
            bind(&WeightsModel::handleActiveXformContent, &weightsModel, _1)
        ));
        connections.push_back(flameView->frameContent.connect(
            bind(&Renderer::handleFrameContent, &renderer, _1)
        ));
        connections.push_back(flameView->activeXformContent.connect(
            bind(&ColorModel::handleActiveXformContent, &colorModel, _1)
        ));
        connections.push_back(triangleModel.xformSelected.connect(
            bind(&FlameView::handleXFormSelected, flameView, _1)
        ));
        connections.push_back(triangleModel.xformUpdate.connect(
            bind(&FlameView::handleXFormUpdate, flameView, _1)
        ));
        connections.push_back(triangleModel.startXFormExplore.connect(
            bind(&FlameView::handleStartXFormExplore, flameView)
        ));
        connections.push_back(triangleModel.stopXFormExplore.connect(
            bind(&FlameView::handleStopXFormExplore, flameView)
        ));
        connections.push_back(preTransformModel.xformUpdate.connect(
            bind(&FlameView::handleXFormUpdate, flameView, _1)
        ));
        connections.push_back(postTransformModel.xformUpdate.connect(
            bind(&FlameView::handleXFormUpdate, flameView, _1)
        ));
        connections.push_back(weightsModel.xformSelected.connect(
            bind(&FlameView::handleXFormSelected, flameView, _1)
        ));
        connections.push_back(weightsModel.xformAdded.connect(
            bind(&FlameView::handleXFormAdded, flameView, _1)
        ));
        connections.push_back(weightsModel.xformRemoved.connect(
            bind(&FlameView::handleXFormRemoved, flameView, _1)
        ));
        connections.push_back(weightsModel.xformUpdate.connect(
            bind(&FlameView::handleXFormUpdate, flameView, _1)
        ));
        connections.push_back(variationModel.xformUpdate.connect(
            bind(&FlameView::handleXFormUpdate, flameView, _1)
        ));
        connections.push_back(colorModel.xformUpdate.connect(
            bind(&FlameView::handleXFormUpdate, flameView, _1)
        ));
        connections.push_back(colorModel.startXFormExplore.connect(
            bind(&FlameView::handleStartXFormExplore, flameView)
        ));
        connections.push_back(colorModel.stopXFormExplore.connect(
            bind(&FlameView::handleStopXFormExplore, flameView)
        ));
        connections.push_back(frameModel.frameContent.connect(
            bind(&FlameView::handleFrameContent, flameView, _1)
        ));
    }
}

void AevusFrame::onPaletteEditorSelected(wxCommandEvent& event) {
    if (menuBar->IsChecked(ID_PALETTE_EDITOR)) {
        std::optional<core::Gradient> flameGradient = std::nullopt;
        if (flameView != NULL) {
            flameGradient = flameView->getGradient();
        }
        paletteFrame = new PaletteFrame(this, flameView, &presetLibrary);
        paletteFrame->Show(true);
    } else {
        paletteFrame->Close(true);
    }
}
 
void AevusFrame::onPaletteEditorClosed() {
    menuBar->Check(ID_PALETTE_EDITOR, false);
}

void AevusFrame::onUndo(wxCommandEvent& event) {
    docManager->GetCurrentDocument()->GetCommandProcessor()->Undo();
}

void AevusFrame::onRedo(wxCommandEvent& event) {
    docManager->GetCurrentDocument()->GetCommandProcessor()->Redo();
}

wxBitmap AevusFrame::loadEmbeddedPNG(char* start, char* end) {
    size_t sz = end - start;
    wxMemoryInputStream stream(start, sz);
    wxImage image(stream, wxBITMAP_TYPE_PNG);
    return wxBitmap(image);
}

void AevusFrame::onTransformValueChanged(wxDataViewEvent& event) {
    switch (event.GetId()) {
        case ID_PRE_DATAVIEW: preTransformModel.handleValueChangedEvent(event); break;
        case ID_POST_DATAVIEW: postTransformModel.handleValueChangedEvent(event); break;
    }
}

void AevusFrame::onTransformSelected(wxDataViewEvent& event) {
    weightsModel.handleSelectionEvent(event);
}

void AevusFrame::onWeightEdited(wxDataViewEvent& event) {
    weightsModel.handleValueChangedEvent(event);
}

void AevusFrame::onButtonClick(wxCommandEvent& event) {
    int eventId = event.GetId();
    switch (eventId) {
        case ID_PRE_RESET_BUTTON: preTransformModel.handleReset(); break;
        case ID_POST_RESET_BUTTON: postTransformModel.handleReset(); break;
        case ID_ADD_XFORM_BUTTON: weightsModel.handleAddXform(); break;
        case ID_REMOVE_XFORM_BUTTON: weightsModel.handleRemoveXform(); break;
    }
}

void AevusFrame::onVariationAddEnter(wxCommandEvent& event) {
    variationModel.handleVariationAdd();
}

void AevusFrame::onVariationValueChanged(wxDataViewEvent& event) {
    variationModel.handleValueChangedEvent(event);
}

void AevusFrame::onColorValueChanged(wxDataViewEvent& event) {
    colorModel.handleValueChangedEvent(event);
}

void AevusFrame::onFrameValueChanged(wxDataViewEvent& event) {
    frameModel.handleValueChangedEvent(event);
}

void AevusFrame::onPaint(wxPaintEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_WINDOW: flameModel.handlePaint(); break;
        case ID_PALETTE_PANEL: colorModel.handlePaint(); break;
        case ID_TRIANGLE_PANEL: triangleModel.handlePaint(); break;
    }
}

void AevusFrame::onResize(wxSizeEvent& event) {
    switch (event.GetId()) {
        case ID_TRIANGLE_PANEL: triangleModel.handleResize(event); break;
    }
}

void AevusFrame::onDataViewLostFocus(wxFocusEvent& event) {
    switch (event.GetId()) {
        case ID_PRE_DATAVIEW: preTransformModel.handleKillFocusEvent(event); break;
        case ID_POST_DATAVIEW: postTransformModel.handleKillFocusEvent(event); break;
    }
}

void AevusFrame::onMouseDown(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_PALETTE_PANEL: colorModel.handleMouseDown(event); break;
        case ID_TRIANGLE_PANEL: triangleModel.handleMouseDown(event); break;
    }
}

void AevusFrame::onActivate(wxActivateEvent& event) {
    if (!event.GetActive()) {
        colorModel.handleMouseUp();
        triangleModel.handleMouseUp();
    }
}

void AevusFrame::onMouseUp(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_PALETTE_PANEL: colorModel.handleMouseUp(); break;
        case ID_TRIANGLE_PANEL: triangleModel.handleMouseUp(); break;
    }
}

void AevusFrame::onMouseMove(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_PALETTE_PANEL: colorModel.handleMouseMove(event); break;
        case ID_TRIANGLE_PANEL: triangleModel.handleMouseMove(event); break;
    }
}

void AevusFrame::onMouseWheel(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_TRIANGLE_PANEL: triangleModel.handleMouseWheel(event); break;
        case ID_FLAME_WINDOW: flameModel.handleMouseWheel(event); break;
    }
}

void AevusFrame::onExit(wxCommandEvent& event) {
    Close(true);
}

void AevusFrame::onAbout(wxCommandEvent& event) {
    editMenu->Enable(wxID_UNDO, true);
    wxMessageBox("This is an early alpha of the Aevus fractal flame editor.",
        "About Aevus", wxOK | wxICON_INFORMATION);
}

}