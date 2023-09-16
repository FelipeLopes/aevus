#include "aevus_frame.hpp"
#include <wx/gdicmn.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/mstream.h>
#include <wx/wfstream.h>
#include "../render/iterator.hpp"
#include "flame_model.hpp"
#include "transform_model.hpp"

using namespace boost::signals2;
using boost::bind;
using clwrap::OpenCL;
using render::Renderer;
using std::optional;
using std::string;

namespace ui {

AevusFrame::AevusFrame(OpenCL* openCL, optional<string> filename): WxfbFrame(NULL),
    context(openCL->createQueuedContext(0, 1)),
    renderer(context, &flame, flameStream),
    flameModel(flameWindow, flameStream),
    preTransformModel(&flame, preTransformDataViewCtrl, resetPreButton, true),
    postTransformModel(&flame, postTransformDataViewCtrl, resetPostButton, false),
    weightsModel(&flame, weightsDataViewCtrl, removeXformButton),
    variationModel(&flame, variationListCtrl, variationTextCtrl),
    colorModel(&flame, colorListCtrl, palettePanel),
    frameModel(&flame, frameListCtrl),
    triangleModel(&flame, trianglePanel)
{
    preTransformModel.transformCoordsChanged
        .connect(eventBroker.activeXformCoordsChanged);
    postTransformModel.transformCoordsChanged
        .connect(eventBroker.activeXformCoordsChanged);
    weightsModel.weightsChanged
        .connect(eventBroker.flameWeightsChanged);
    weightsModel.xformSelected
        .connect(eventBroker.activeXformChanged);
    variationModel.variationDataChanged
        .connect(eventBroker.variationParamsChanged);
    colorModel.colorChanged
        .connect(eventBroker.colorParamsChanged);
    frameModel.frameChanged
        .connect(eventBroker.frameParamsChanged);
    triangleModel.xformSelected
        .connect(eventBroker.activeXformChanged);
    triangleModel.transformCoordsChanged
        .connect(eventBroker.activeXformCoordsChanged);

    renderer.imageRendered.connect(bind(&FlameModel::update, &flameModel));

    eventBroker.flameLoaded
        .connect(bind(&TransformModel::update, &preTransformModel));
    eventBroker.flameLoaded
        .connect(bind(&TransformModel::update, &postTransformModel));
    eventBroker.flameLoaded
        .connect(bind(&TriangleModel::update, &triangleModel));
    eventBroker.flameLoaded
        .connect(bind(&Renderer::renderFlame, &renderer));
    eventBroker.flameLoaded
        .connect(bind(&WeightsModel::update, &weightsModel));
    eventBroker.flameLoaded
        .connect(bind(&VariationModel::update, &variationModel));
    eventBroker.flameLoaded
        .connect(bind(&ColorModel::setupPalette, &colorModel));
    eventBroker.flameLoaded
        .connect(bind(&ColorModel::update, &colorModel));
    eventBroker.flameLoaded
        .connect(bind(&FrameModel::update, &frameModel));

    eventBroker.activeXformCoordsChanged
        .connect(bind(&TransformModel::update, &preTransformModel));
    eventBroker.activeXformCoordsChanged
        .connect(bind(&TransformModel::update, &postTransformModel));
    eventBroker.activeXformCoordsChanged
        .connect(bind(&TriangleModel::update, &triangleModel));
    eventBroker.activeXformCoordsChanged
        .connect(bind(&Renderer::renderFlame, &renderer));
    eventBroker.flameWeightsChanged
        .connect(bind(&WeightsModel::update, &weightsModel));
    eventBroker.flameWeightsChanged
        .connect(bind(&Renderer::renderFlame, &renderer));
    eventBroker.activeXformChanged
        .connect(bind(&TransformModel::handleActiveXformChanged, &preTransformModel, _1));
    eventBroker.activeXformChanged
        .connect(bind(&TransformModel::handleActiveXformChanged, &postTransformModel, _1));
    eventBroker.activeXformChanged
        .connect(bind(&WeightsModel::handleActiveXformChanged, &weightsModel, _1));
    eventBroker.activeXformChanged
        .connect(bind(&VariationModel::handleActiveXformChanged, &variationModel, _1));
    eventBroker.activeXformChanged
        .connect(bind(&ColorModel::handleActiveXformChanged, &colorModel, _1));
    eventBroker.activeXformChanged
        .connect(bind(&TriangleModel::handleActiveXformChanged, &triangleModel, _1));
    eventBroker.variationParamsChanged
        .connect(bind(&VariationModel::update, &variationModel));
    eventBroker.variationParamsChanged
        .connect(bind(&Renderer::renderFlame, &renderer));
    eventBroker.paletteChanged
        .connect(bind(&ColorModel::setupPalette, &colorModel));
    eventBroker.paletteChanged
        .connect(bind(&Renderer::renderFlame, &renderer));
    eventBroker.colorParamsChanged
        .connect(bind(&ColorModel::update, &colorModel));
    eventBroker.colorParamsChanged
        .connect(bind(&Renderer::renderFlame, &renderer));
    eventBroker.frameParamsChanged
        .connect(bind(&FrameModel::update, &frameModel));
    eventBroker.frameParamsChanged
        .connect(bind(&Renderer::renderFlame, &renderer));

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
        loadFlame(filename.value());
    }
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

void AevusFrame::onMouseUp(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_PALETTE_PANEL: colorModel.handleMouseUp(event); break;
        case ID_TRIANGLE_PANEL: triangleModel.handleMouseUp(event); break;
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
    wxMessageBox("This is an early alpha of the Aevus fractal flame editor.",
        "About Aevus", wxOK | wxICON_INFORMATION);
}

void AevusFrame::loadFlame(std::string filename) {
    FILE* inputStream = fopen(filename.c_str(), "r");
    if (inputStream == NULL) {
        printf("Error on opening file: %s\n", filename.c_str());
        return;
    }
    flame.deserialize(inputStream);
    fclose(inputStream);
    eventBroker.flameLoaded();
    if (flame.xforms.size() > 0) {
        eventBroker.activeXformChanged(0);
    }
}

void AevusFrame::onFileOpen(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open flame file", "", "",
        "Flame files (*.flame)|*.flame|XML files|*.xml",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    auto filename = openFileDialog.GetPath().ToStdString();
    loadFlame(filename);
}

void AevusFrame::onFileSaveAs(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, "Save flame file", "", "",
        "XML files (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    auto filename = saveFileDialog.GetPath().ToStdString();
    FILE* outputStream = fopen(filename.c_str(), "w");
    if (outputStream == NULL) {
        printf("Error on opening file: %s\n", filename.c_str());
        return;
    }
    flame.serialize(outputStream);
    fclose(outputStream);
}

}