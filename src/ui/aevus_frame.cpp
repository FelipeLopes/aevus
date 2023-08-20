#include "aevus_frame.hpp"
#include "event_broker.hpp"
#include "transform_model.hpp"
#include "variation_text_completer.hpp"
#include "wxfb/code/wxfb_frame.h"
#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

using namespace boost::signals2;
using boost::bind;

using std::string;
using std::to_string;

namespace ui {

wxDEFINE_EVENT(FLAME_UPDATE_EVENT, wxCommandEvent);
wxDEFINE_EVENT(FLAME_XFORM_CHANGE_EVENT, wxCommandEvent);

AevusFrame::AevusFrame(std::shared_ptr<core::Flame> flame_): WxfbFrame(NULL),
    flame(flame_)
{
    SetStatusText("Welcome to Aevus!");
    Bind(FLAME_UPDATE_EVENT, &AevusFrame::onFlameUpdate, this, wxID_ANY);
    Bind(FLAME_XFORM_CHANGE_EVENT, &AevusFrame::onFlameXformChange, this, wxID_ANY);
    editingId = -1;
    editingTransform = -1;
    variationTextCtrl->AutoComplete(new VariationTextCompleter);
    eventBroker = std::make_shared<EventBroker>();
    preTransformModel =
        std::make_shared<TransformModel>(flame, this, preTransformDataViewCtrl, true);
    postTransformModel =
        std::make_shared<TransformModel>(flame, this, postTransformDataViewCtrl, false);
    weightsModel = std::make_shared<WeightsModel>(flame, this, weightsDataViewCtrl);
    variationModel = std::make_shared<VariationModel>(variationListCtrl);

    fileLoaded.connect(bind(&TransformModel::update, preTransformModel));
    fileLoaded.connect(bind(&TransformModel::update, postTransformModel));
    fileLoaded.connect(bind(&WeightsModel::update, weightsModel));

    preTransformModel->transformValueChanged
        .connect(bind(&EventBroker::preTransformValueChanged, eventBroker));
    postTransformModel->transformValueChanged
        .connect(bind(&EventBroker::postTransformValueChanged, eventBroker));

    eventBroker->activeXformValueChanged
        .connect(bind(&TransformModel::update, preTransformModel));
    eventBroker->activeXformValueChanged
        .connect(bind(&TransformModel::update, postTransformModel));

    loadFile("../in.xml");

    variationModel->update();

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

void AevusFrame::fireFlameUpdateEvent() {
    wxCommandEvent flameUpdateEvent(FLAME_UPDATE_EVENT, wxID_ANY);
    flameUpdateEvent.SetEventObject(this);
    ProcessWindowEvent(flameUpdateEvent);
}

void AevusFrame::fireFlameXformChangeEvent() {
    wxCommandEvent flameXformChangeEvent(FLAME_XFORM_CHANGE_EVENT, wxID_ANY);
    flameXformChangeEvent.SetEventObject(this);
    flameXformChangeEvent.SetInt(editingTransform);
    ProcessWindowEvent(flameXformChangeEvent);
}

void AevusFrame::onResetFlameUpdate(wxCommandEvent& event) {
    int eventId = event.GetId();
    switch (eventId) {
        case ID_FLAME_PRE_RESET: preTransformModel->handleReset(); break;
        case ID_FLAME_POST_RESET: postTransformModel->handleReset(); break;
    }
}

void AevusFrame::onVariationAddEnter(wxCommandEvent& event) {
    string text = variationTextCtrl->GetValue().ToStdString();
    auto validNames = core::Variation::variationNames.right;
    auto nameIt = validNames.find(text);
    if (nameIt == validNames.end()) {
        return;
    }
    auto id = nameIt->second;
    auto vars = flame->xforms.get(editingTransform)->variationMap.getValue().variations;
    if (vars.find(id) != vars.end()) {
        return;
    }
    vars[id] = 1.0;
    core::VariationMap newVarMap;
    newVarMap.variations = vars;
    flame->xforms.get(editingTransform)->variationMap.setValue(newVarMap);
    variationTextCtrl->ChangeValue("");
    fireFlameUpdateEvent();
}

void AevusFrame::onVariationValueChanged(wxDataViewEvent& event) {
    auto item = event.GetItem();
    int row = variationListCtrl->ItemToRow(item);
    string variation = variationListCtrl->GetTextValue(row, 0).ToStdString();
    string text = variationListCtrl->GetTextValue(row, 1).ToStdString();
    auto vars = flame->xforms.get(editingTransform)->variationMap.getValue().variations;
    auto it = core::Variation::variationNames.right.find(variation);
    if (it == core::Variation::variationNames.right.end()) {
        throw std::invalid_argument("Invalid variation name");
    }
    auto id = it->second;
    try {
        double value = std::stod(text);
        if (value == 0.0) {
            vars.erase(id);
        } else {
            vars[id] = value;
        }
        core::VariationMap newVarMap;
        newVarMap.variations = vars;
        flame->xforms.get(editingTransform)->variationMap.setValue(newVarMap);
        fireFlameUpdateEvent();
    } catch (std::invalid_argument& e) {
        double val = vars[id];
        variationListCtrl->SetValue(to_string(val), row, 1);
    }
}

void AevusFrame::onDataViewLostFocus(wxFocusEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_PRE_DV: preTransformModel->handleKillFocusEvent(event); break;
        case ID_FLAME_POST_DV: postTransformModel->handleKillFocusEvent(event); break;
    }
}

void AevusFrame::onFlameUpdate(wxCommandEvent& event) {
    if (flame->xforms.size() == 0) {
        transformsScrolledWindow->Disable();
        return;
    }
    transformsScrolledWindow->Enable();
    auto varMap = flame->xforms.get(editingTransform)->variationMap.getValue().variations;
    // HACK: we delete each item individually instead of calling DeleteAllItems()
    // because DeleteAllItems seems to be bugged, it changes the configuration
    // of the data view ctrl to enable the search textbox in Ubuntu, and we don't want it.
    for (int i=variationListCtrl->GetItemCount(); i>0; i--) {
        variationListCtrl->DeleteItem(i-1);
    }
    for (auto el: varMap) {
        wxVector<wxVariant> item;
        auto varNameIt = core::Variation::variationNames.left.find(el.first);
        if (varNameIt == core::Variation::variationNames.left.end()) {
            throw std::invalid_argument("Unknown variation ID");
        }
        item.push_back(wxVariant(varNameIt->second));
        item.push_back(wxVariant(to_string(el.second)));
        variationListCtrl->AppendItem(item);
    }
    preTransformModel->update();
    postTransformModel->update();
    weightsModel->update();
}

void AevusFrame::onFlameXformChange(wxCommandEvent& event) {
    preTransformModel->handleActiveFormChangedEvent(event);
    postTransformModel->handleActiveFormChangedEvent(event);
}

void AevusFrame::onExit(wxCommandEvent& event) {
    Close(true);
}

void AevusFrame::onAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets' Hello world sample",
        "About Hello World", wxOK | wxICON_INFORMATION);
}

void AevusFrame::loadFile(std::string filename) {
    FILE* inputStream = fopen(filename.c_str(), "r");
    if (inputStream == NULL) {
        printf("Error on opening file: %s\n", filename.c_str());
        return;
    }
    flame->deserialize(inputStream);
    fclose(inputStream);
    editingTransform = 0;
    fileLoaded();
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