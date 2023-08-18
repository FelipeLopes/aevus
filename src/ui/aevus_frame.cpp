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

AevusFrame::AevusFrame(std::shared_ptr<core::Flame> flame_): WxfbFrame(NULL),
    flame(flame_)
{
    SetStatusText("Welcome to Aevus!");
    Bind(FLAME_UPDATE_EVENT, &AevusFrame::onFlameUpdate, this, wxID_ANY);
    editingId = -1;
    textCtrls = {
        preXXtextCtrl, preXYtextCtrl, preYXtextCtrl,
        preYYtextCtrl, preOXtextCtrl, preOYtextCtrl,
        postXXtextCtrl, postXYtextCtrl, postYXtextCtrl,
        postYYtextCtrl, postOXtextCtrl, postOYtextCtrl
    };
    editingTransform = -1;
    loadFile("../in.xml");
    variationTextCtrl->AutoComplete(new VariationTextCompleter);
    transformModel = std::make_shared<TransformModel>(this, transformDataViewCtrl);
    transformModel->update();
}

void AevusFrame::onTransformValueChanged(wxDataViewEvent& event) {
    transformModel->receiveValueChangedEvent(event);
}

int AevusFrame::getCoefIndexByTextCtrlId(int textCtrlId) {
    switch (textCtrlId) {
        case ID_FLAME_PRE_XX: return 0;
        case ID_FLAME_PRE_XY: return 1;
        case ID_FLAME_PRE_YX: return 2;
        case ID_FLAME_PRE_YY: return 3;
        case ID_FLAME_PRE_OX: return 4;
        case ID_FLAME_PRE_OY: return 5;
        case ID_FLAME_POST_XX: return 6;
        case ID_FLAME_POST_XY: return 7;
        case ID_FLAME_POST_YX: return 8;
        case ID_FLAME_POST_YY: return 9;
        case ID_FLAME_POST_OX: return 10;
        case ID_FLAME_POST_OY: return 11;
        default: return -1;
    }
}

void AevusFrame::onTransformChosen(wxCommandEvent& event) {
    int chosen = transformChoice->GetSelection();
    if (chosen != editingTransform) {
        editingTransform = chosen;
        fireFlameUpdateEvent();
    }
}

void AevusFrame::fireFlameUpdateEvent() {
    wxCommandEvent flameUpdateEvent(FLAME_UPDATE_EVENT, wxID_ANY);
    flameUpdateEvent.SetEventObject(this);
    ProcessWindowEvent(flameUpdateEvent);
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

void AevusFrame::onEnterFlameUpdate(wxCommandEvent& event) {
    int enterId = event.GetId();
    if (!flameTextEqual(enterId)) {
        tryChangeAndUpdate(enterId);
    }
}

void AevusFrame::onFocusFlameEdit(wxFocusEvent& event) {
    int focusId = event.GetId();
    if (editingId != focusId) {
        if (!flameTextEqual(editingId)) {
            tryChangeAndUpdate(editingId);
        }
    }
    editingId = focusId;
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

bool AevusFrame::tryChangeAndUpdate(int textCtrlId) {
    string text;
    int coefId = getCoefIndexByTextCtrlId(textCtrlId);
    if (coefId == -1) {
        if (textCtrlId == ID_FLAME_WEIGHT) {
            try {
                double val = std::stod(weightTextCtrl->GetValue().ToStdString());
                flame->xforms.get(editingTransform)->weight.setValue(val);
                fireFlameUpdateEvent();
            } catch (std::exception& e) {
                return false;
            }
        }
        return false;
    }
    text = textCtrls[coefId]->GetValue();
    if (flame->xforms.size() == 0) {
        throw std::invalid_argument("No xforms found");
    }
    try {
        double val = std::stod(text);
        if (coefId / 6 == 0) {
            auto coefs = flame->xforms.get(editingTransform)->coefs.get();
            coefs->setValueByIndex(coefId % 6, val);
        } else {
            auto post = flame->xforms.get(editingTransform)->post.get();
            post->setValueByIndex(coefId % 6, val);
        }
        fireFlameUpdateEvent();
        return true;
    } catch (std::exception& e) {
        return false;
    }
}

bool AevusFrame::flameTextEqual(int textCtrlId) {
    string text;
    int coefId = getCoefIndexByTextCtrlId(textCtrlId);
    if (coefId == -1) {
        if (textCtrlId == ID_FLAME_WEIGHT) {
            return text == weightTextCtrl->GetValue();
        }
        return true;
    }
    text = textCtrls[coefId]->GetValue();
    if (flame->xforms.size() == 0) {
        return true;
    }
    double flameVal;
    if (coefId / 6 == 0) {
        flameVal = flame->xforms.get(editingTransform)->coefs.get()->getValueByIndex(coefId % 6);
    } else {
        flameVal = flame->xforms.get(editingTransform)->post.get()->getValueByIndex(coefId % 6);
    }
    return std::to_string(flameVal) == text;
}

void AevusFrame::onFlameUpdate(wxCommandEvent& event) {
    printf("Event received!\n");
    if (flame->xforms.size() == 0) {
        transformsScrolledWindow->Disable();
        return;
    }
    auto coefs = flame->xforms.get(editingTransform)->coefs.get();
    auto post = flame->xforms.get(editingTransform)->post.get();
    for (int i=0; i<6; i++) {
        textCtrls[i]->ChangeValue(to_string(coefs->getValueByIndex(i)));
        textCtrls[i+6]->ChangeValue(to_string(post->getValueByIndex(i)));
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