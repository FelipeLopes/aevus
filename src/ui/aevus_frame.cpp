#include "aevus_frame.hpp"
#include "color_model.hpp"
#include "event_broker.hpp"
#include "transform_model.hpp"
#include "variation_model.hpp"
#include "weights_model.hpp"
#include "wxfb/code/wxfb_frame.h"
#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <wx/rawbmp.h>

using namespace boost::signals2;
using boost::bind;

using std::string;

namespace ui {

AevusFrame::AevusFrame(std::shared_ptr<core::Flame> flame_): WxfbFrame(NULL),
    flame(flame_)
{
    SetStatusText("Welcome to Aevus!");

    eventBroker = std::make_shared<EventBroker>();

    preTransformModel =
        std::make_shared<TransformModel>(flame, preTransformDataViewCtrl, true);
    postTransformModel =
        std::make_shared<TransformModel>(flame, postTransformDataViewCtrl, false);
    weightsModel = std::make_shared<WeightsModel>(flame, weightsDataViewCtrl);
    variationModel =
        std::make_shared<VariationModel>(flame, variationListCtrl, variationTextCtrl);
    colorModel = std::make_shared<ColorModel>(flame, colorListCtrl, palettePanel);

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

    eventBroker->activeXformCoordsChanged
        .connect(bind(&TransformModel::update, preTransformModel));
    eventBroker->activeXformCoordsChanged
        .connect(bind(&TransformModel::update, postTransformModel));
    eventBroker->flameWeightsChanged
        .connect(bind(&WeightsModel::update, weightsModel));
    eventBroker->activeXformChanged
        .connect(bind(&TransformModel::handleActiveXformChanged, preTransformModel, _1));
    eventBroker->activeXformChanged
        .connect(bind(&TransformModel::handleActiveXformChanged, postTransformModel, _1));
    eventBroker->activeXformChanged
        .connect(bind(&VariationModel::handleActiveXformChanged, variationModel, _1));
    eventBroker->activeXformChanged
        .connect(bind(&ColorModel::handleActiveXformChanged, colorModel, _1));
    eventBroker->variationParamsChanged
        .connect(bind(&VariationModel::update, variationModel));
    eventBroker->paletteChanged
        .connect(bind(&ColorModel::setupPalette, colorModel));
    eventBroker->colorParamsChanged
        .connect(bind(&ColorModel::update, colorModel));

    loadFile("../in.xml");

    colorModel->update();

    trianglePanel->SetFocus();
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

void AevusFrame::onResetFlameUpdate(wxCommandEvent& event) {
    int eventId = event.GetId();
    switch (eventId) {
        case ID_FLAME_PRE_RESET: preTransformModel->handleReset(); break;
        case ID_FLAME_POST_RESET: postTransformModel->handleReset(); break;
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

void AevusFrame::onPalettePaint(wxPaintEvent& event) {
    colorModel->handlePaint();
}

void AevusFrame::onDataViewLostFocus(wxFocusEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_PRE_DV: preTransformModel->handleKillFocusEvent(event); break;
        case ID_FLAME_POST_DV: postTransformModel->handleKillFocusEvent(event); break;
    }
}

void AevusFrame::onMouseLeave(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_PALETTE_PANEL: colorModel->handleMouseLeave(event); break;
    }
}

void AevusFrame::onMouseDown(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_PALETTE_PANEL: colorModel->handleMouseDown(event); break;
    }
}

void AevusFrame::onMouseUp(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_PALETTE_PANEL: colorModel->handleMouseUp(event); break;
    }
}

void AevusFrame::onMouseMove(wxMouseEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_PALETTE_PANEL: colorModel->handleMouseMove(event); break;
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
    flame->deserialize(inputStream);
    fclose(inputStream);
    eventBroker->paletteChanged();
    eventBroker->activeXformChanged(0);
    eventBroker->flameWeightsChanged();
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
    flame->serialize(outputStream);
    fclose(outputStream);
}

AevusFrame::~AevusFrame() { }

bool Aevus::OnInit()
{
    auto flame = std::make_shared<core::Flame>();
    AevusFrame* frame = new AevusFrame(flame);
    frame->Show();
    return true;
}

}