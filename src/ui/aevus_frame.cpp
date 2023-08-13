#include "aevus_frame.hpp"
#include "wxfb/code/wxfb_frame.h"
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <wx-3.2/wx/event.h>
#include <wx-3.2/wx/filedlg.h>
#include <wx-3.2/wx/gtk/filedlg.h>

using std::string;
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
        flame->xforms.get(0)->coefs.setValue(coefs);
    } else if (eventId == ID_FLAME_POST_RESET) {
        core::PostAffine post;
        if (flame->xforms.size() == 0) {
            return;
        }
        flame->xforms.get(0)->post.setValue(post);
    } else {
        throw std::invalid_argument("Unrecognized ID");
    }
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

bool AevusFrame::tryChangeAndUpdate(int textCtrlId) {
    string text;
    int coefId = getCoefIndexByTextCtrlId(textCtrlId);
    if (coefId == -1) {
        return false;
    }
    text = textCtrls[coefId]->GetValue();
    if (flame->xforms.size() == 0) {
        throw std::invalid_argument("No xforms found");
    }
    try {
        double val = std::stod(text);
        if (coefId / 6 == 0) {
            auto coefs = flame->xforms.get(editingTransform)->coefs.getValue();
            coefs.setValueByIndex(coefId % 6, val);
            flame->xforms.get(editingTransform)->coefs.setValue(coefs);
        } else {
            auto post = flame->xforms.get(editingTransform)->post.getValue();
            post.setValueByIndex(coefId % 6, val);
            flame->xforms.get(editingTransform)->post.setValue(post);
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
        return true;
    }
    text = textCtrls[coefId]->GetValue();
    if (flame->xforms.size() == 0) {
        return true;
    }
    double flameVal;
    if (coefId / 6 == 0) {
        flameVal = flame->xforms.get(editingTransform)->coefs.getValue().getValueByIndex(coefId % 6);
    } else {
        flameVal = flame->xforms.get(editingTransform)->post.getValue().getValueByIndex(coefId % 6);
    }
    return std::to_string(flameVal) == text;
}

void AevusFrame::onFlameUpdate(wxCommandEvent& event) {
    if (flame->xforms.size() == 0) {
        transformsScrolledWindow->Disable();
        return;
    }
    auto coefs = flame->xforms.get(editingTransform)->coefs.getValue();
    auto post = flame->xforms.get(editingTransform)->post.getValue();
    for (int i=0; i<6; i++) {
        textCtrls[i]->ChangeValue(to_string(coefs.getValueByIndex(i)));
        textCtrls[i+6]->ChangeValue(to_string(post.getValueByIndex(i)));
    }
    transformsScrolledWindow->Enable();
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

}