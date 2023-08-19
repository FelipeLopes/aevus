#include "aevus_frame.hpp"
#include "transform_model.hpp"
#include "wxfb/code/wxfb_frame.h"
#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

using std::string;
using std::vector;
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
    preTransformModel =
        std::make_shared<TransformModel>(flame, this, preTransformDataViewCtrl, true);
    postTransformModel =
        std::make_shared<TransformModel>(flame, this, postTransformDataViewCtrl, false);
    weightsModel = std::make_shared<WeightsModel>(weightsDataViewCtrl);
    loadFile("../in.xml");
}

void AevusFrame::onTransformValueChanged(wxDataViewEvent& event) {
    switch (event.GetId()) {
        case ID_FLAME_PRE_DV: preTransformModel->handleValueChangedEvent(event); break;
        case ID_FLAME_POST_DV: postTransformModel->handleValueChangedEvent(event); break;
    }
}

void AevusFrame::onTransformChosen(wxCommandEvent& event) {
    int chosen = transformChoice->GetSelection();
    if (chosen != editingTransform) {
        editingTransform = chosen;
        fireFlameUpdateEvent();
        fireFlameXformChangeEvent();
    }
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
    if (eventId == ID_FLAME_PRE_RESET) {
        core::CoefsAffine coefs;
        if (flame->xforms.size() == 0) {
            return;
        }
        auto flameCoefs = flame->xforms.get(0)->coefs.get();
        *flameCoefs = coefs;
    } else if (eventId == ID_FLAME_POST_RESET) {
        core::PostAffine post;
        if (flame->xforms.size() == 0) {
            return;
        }
        auto flamePost = flame->xforms.get(0)->post.get();
        *flamePost = post;
    } else {
        throw std::invalid_argument("Unrecognized ID");
    }
    fireFlameUpdateEvent();
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
    double weight = flame->xforms.get(editingTransform)->weight.getValue();
    weightTextCtrl->ChangeValue(to_string(weight));
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
    for (int i=0; i<flame->xforms.size(); i++) {
        transformChoice->Append(to_string(i+1));
    }
    editingTransform = 0;
    transformChoice->SetSelection(0);
    fireFlameUpdateEvent();
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

VariationTextCompleter::VariationTextCompleter() {
    for (auto el: core::Variation::variationNames.right) {
        validStrings.insert(el.first);
    }
}

void VariationTextCompleter::GetCompletions(const wxString& prefix, wxArrayString& res) {
    vector<string> ans;
    string pref = prefix.ToStdString();
    if (pref.size() == 0) {
        return;
    }
    std::copy_if(validStrings.begin(), validStrings.end(), std::back_inserter(ans), [&](string s) {
        return s.starts_with(pref);
    });
    if (ans.size() == 1 && pref.compare(ans[0]) == 0) {
        ans.clear();
    }
    for (auto el: ans) {
        res.push_back(el);
    }
}

}