#include "aevus_frame.hpp"
#include <wx/gdicmn.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/mstream.h>

using namespace boost::signals2;
using boost::bind;
using std::make_shared;
using std::string;

namespace ui {

AevusFrame::AevusFrame(): WxfbFrame(NULL) {
    SetStatusText("Welcome to Aevus!");

    eventBroker = make_shared<EventBroker>();

    preTransformModel = make_shared<TransformModel>(&flame, preTransformDataViewCtrl,
        resetPreButton, true);
    postTransformModel = make_shared<TransformModel>(&flame, postTransformDataViewCtrl,
        resetPostButton, false);
    weightsModel = make_shared<WeightsModel>(&flame, weightsDataViewCtrl, removeXformButton);
    variationModel = make_shared<VariationModel>(&flame, variationListCtrl, variationTextCtrl);
    colorModel = make_shared<ColorModel>(&flame, colorListCtrl, palettePanel);
    frameModel = make_shared<FrameModel>(&flame, frameListCtrl);
    triangleModel = make_shared<TriangleModel>(&flame, trianglePanel);

    preTransformModel->transformCoordsChanged
        .connect(eventBroker->activeXformCoordsChanged);
    postTransformModel->transformCoordsChanged
        .connect(eventBroker->activeXformCoordsChanged);
    weightsModel->weightsChanged
        .connect(eventBroker->flameWeightsChanged);
    weightsModel->xformSelected
        .connect(eventBroker->activeXformChanged);
    variationModel->variationDataChanged
        .connect(eventBroker->variationParamsChanged);
    colorModel->colorChanged
        .connect(eventBroker->colorParamsChanged);
    frameModel->frameChanged
        .connect(eventBroker->frameParamsChanged);
    triangleModel->xformSelected
        .connect(eventBroker->activeXformChanged);
    triangleModel->transformCoordsChanged
        .connect(eventBroker->activeXformCoordsChanged);

    eventBroker->activeXformCoordsChanged
        .connect(bind(&TransformModel::update, preTransformModel));
    eventBroker->activeXformCoordsChanged
        .connect(bind(&TransformModel::update, postTransformModel));
    eventBroker->activeXformCoordsChanged
        .connect(bind(&TriangleModel::update, triangleModel));
    eventBroker->flameWeightsChanged
        .connect(bind(&WeightsModel::update, weightsModel));
    eventBroker->activeXformChanged
        .connect(bind(&TransformModel::handleActiveXformChanged, preTransformModel, _1));
    eventBroker->activeXformChanged
        .connect(bind(&TransformModel::handleActiveXformChanged, postTransformModel, _1));
    eventBroker->activeXformChanged
        .connect(bind(&WeightsModel::handleActiveXformChanged, weightsModel, _1));
    eventBroker->activeXformChanged
        .connect(bind(&VariationModel::handleActiveXformChanged, variationModel, _1));
    eventBroker->activeXformChanged
        .connect(bind(&ColorModel::handleActiveXformChanged, colorModel, _1));
    eventBroker->activeXformChanged
        .connect(bind(&TriangleModel::handleActiveXformChanged, triangleModel, _1));
    eventBroker->variationParamsChanged
        .connect(bind(&VariationModel::update, variationModel));
    eventBroker->paletteChanged
        .connect(bind(&ColorModel::setupPalette, colorModel));
    eventBroker->colorParamsChanged
        .connect(bind(&ColorModel::update, colorModel));
    eventBroker->frameParamsChanged
        .connect(bind(&FrameModel::update, frameModel));

    trianglePanel->SetFocus();

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
    removeXformButton->SetBitmap(loadEmbeddedPNG(
        _binary_res_minus_disabled_png_start,
        _binary_res_minus_disabled_png_end
    ));
}

wxBitmap AevusFrame::loadEmbeddedPNG(char* start, char* end) {
    size_t sz = end - start;
    wxMemoryInputStream stream(start, sz);
    wxImage image(stream, wxBITMAP_TYPE_PNG);
    return wxBitmap(image);
}

void AevusFrame::onTransformValueChanged(wxDataViewEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_PRE_DV: preTransformModel->handleValueChangedEvent(event); break;
        case ID_FLAME_POST_DV: postTransformModel->handleValueChangedEvent(event); break;
    }
}

void AevusFrame::onTransformSelected(wxDataViewEvent& event) {
    weightsModel->handleSelectionEvent(event);
}

void AevusFrame::onWeightEdited(wxDataViewEvent& event) {
    weightsModel->handleValueChangedEvent(event);
}

void AevusFrame::onButtonClick(wxCommandEvent& event) {
    int eventId = event.GetId();
    switch (eventId) {
        case ID_FLAME_PRE_RESET: preTransformModel->handleReset(); break;
        case ID_FLAME_POST_RESET: postTransformModel->handleReset(); break;
        case ID_FLAME_ADD_XFORM: weightsModel->handleAddXform(); break;
        case ID_FLAME_REMOVE_XFORM: weightsModel->handleRemoveXform(); break;
    }
}

void AevusFrame::onVariationAddEnter(wxCommandEvent& event) {
    variationModel->handleVariationAdd();
}

void AevusFrame::onVariationValueChanged(wxDataViewEvent& event) {
    variationModel->handleValueChangedEvent(event);
}

void AevusFrame::onColorValueChanged(wxDataViewEvent& event) {
    colorModel->handleValueChangedEvent(event);
}

void AevusFrame::onFrameValueChanged(wxDataViewEvent& event) {
    frameModel->handleValueChangedEvent(event);
}

void AevusFrame::onPaint(wxPaintEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_PALETTE_PANEL: colorModel->handlePaint(); break;
        case ID_FLAME_TRIANGLE_PANEL: triangleModel->handlePaint(); break;

    }
}

void AevusFrame::onResize(wxSizeEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_TRIANGLE_PANEL: triangleModel->handleResize(event); break;
    }
}

void AevusFrame::onDataViewLostFocus(wxFocusEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_PRE_DV: preTransformModel->handleKillFocusEvent(event); break;
        case ID_FLAME_POST_DV: postTransformModel->handleKillFocusEvent(event); break;
    }
}

void AevusFrame::onMouseDown(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_PALETTE_PANEL: colorModel->handleMouseDown(event); break;
        case ID_FLAME_TRIANGLE_PANEL: triangleModel->handleMouseDown(event); break;
    }
}

void AevusFrame::onMouseUp(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_PALETTE_PANEL: colorModel->handleMouseUp(event); break;
        case ID_FLAME_TRIANGLE_PANEL: triangleModel->handleMouseUp(event); break;
    }
}

void AevusFrame::onMouseMove(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_PALETTE_PANEL: colorModel->handleMouseMove(event); break;
        case ID_FLAME_TRIANGLE_PANEL: triangleModel->handleMouseMove(event); break;
    }
}

void AevusFrame::onMouseWheel(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_TRIANGLE_PANEL: triangleModel->handleMouseWheel(event); break;
    }
}

void AevusFrame::onExit(wxCommandEvent& event) {
    Close(true);
}

void AevusFrame::onAbout(wxCommandEvent& event) {
    wxMessageBox("This is an early alpha of the Aevus fractal flame editor.",
        "About Aevus", wxOK | wxICON_INFORMATION);
}

void AevusFrame::loadFile(std::string filename) {
    FILE* inputStream = fopen(filename.c_str(), "r");
    if (inputStream == NULL) {
        printf("Error on opening file: %s\n", filename.c_str());
        return;
    }
    flame.deserialize(inputStream);
    fclose(inputStream);
    eventBroker->paletteChanged();
    eventBroker->flameWeightsChanged();
    eventBroker->activeXformChanged(0);
    eventBroker->frameParamsChanged();
}

void AevusFrame::onFileOpen(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open flame file", "", "",
        "Flame files (*.flame)|*.flame|XML files|*.xml",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    auto filename = openFileDialog.GetPath().ToStdString();
    loadFile(filename);
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