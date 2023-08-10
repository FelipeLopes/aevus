#include "aevus_frame.hpp"
#include "wxfb/code/wxfb_frame.h"
#include <exception>
#include <memory>
#include <string>

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
}

void AevusFrame::onResetFlameUpdate(wxCommandEvent& event) {
    core::CoefsAffine coefs;
    if (flame->xforms.size() == 0) {
        return;
    }
    flame->xforms.get(0)->coefs.setValue(coefs);

    wxCommandEvent flameUpdateEvent(FLAME_UPDATE_EVENT, wxID_ANY);
    flameUpdateEvent.SetEventObject(this);
    ProcessWindowEvent(flameUpdateEvent);
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
    switch (textCtrlId) {
        case ID_FLAME_PRE_OX: text = preOXtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_OY: text = preOYtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_XX: text = preXXtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_XY: text = preXYtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_YX: text = preYXtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_YY: text = preYYtextCtrl->GetValue(); break;
        default: return false;
    }
    try {
        double val = std::stod(text);
        if (flame->xforms.size() == 0) {
            return false;
        }
        auto coefs = flame->xforms.get(0)->coefs.getValue();
        switch (textCtrlId) {
            case ID_FLAME_PRE_OX: coefs.ox = val; break;
            case ID_FLAME_PRE_OY: coefs.oy = val; break;
            case ID_FLAME_PRE_XX: coefs.xx = val; break;
            case ID_FLAME_PRE_XY: coefs.xy = val; break;
            case ID_FLAME_PRE_YX: coefs.yx = val; break;
            case ID_FLAME_PRE_YY: coefs.yy = val; break;
            default: return false;
        }
        flame->xforms.get(0)->coefs.setValue(coefs);

        wxCommandEvent flameUpdateEvent(FLAME_UPDATE_EVENT, wxID_ANY);
        flameUpdateEvent.SetEventObject(this);
        ProcessWindowEvent(flameUpdateEvent);

        return true;
    } catch (std::exception& e) {
        return false;
    }
}

bool AevusFrame::flameTextEqual(int textCtrlId) {
    string text;
    switch (textCtrlId) {
        case ID_FLAME_PRE_OX: text = preOXtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_OY: text = preOYtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_XX: text = preXXtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_XY: text = preXYtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_YX: text = preYXtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_YY: text = preYYtextCtrl->GetValue(); break;
        default: return true;
    }
    if (flame->xforms.size() == 0) {
        return text == "";
    }
    double flameVal = 0;
    auto coefs = flame->xforms.get(0)->coefs.getValue();
    switch (textCtrlId) {
        case ID_FLAME_PRE_OX: flameVal = coefs.ox; break;
        case ID_FLAME_PRE_OY: flameVal = coefs.oy; break;
        case ID_FLAME_PRE_XX: flameVal = coefs.xx; break;
        case ID_FLAME_PRE_XY: flameVal = coefs.xy; break;
        case ID_FLAME_PRE_YX: flameVal = coefs.yx; break;
        case ID_FLAME_PRE_YY: flameVal = coefs.yy; break;
        default: return true;
    }
    return std::to_string(flameVal) == text;
}

void AevusFrame::onFlameUpdate(wxCommandEvent& event) {
    auto affine = flame->xforms.get(0)->coefs.getValue();
    preOXtextCtrl->ChangeValue(to_string(affine.ox));
    preOYtextCtrl->ChangeValue(to_string(affine.oy));
    preXXtextCtrl->ChangeValue(to_string(affine.xx));
    preXYtextCtrl->ChangeValue(to_string(affine.xy));
    preYXtextCtrl->ChangeValue(to_string(affine.yx));
    preYYtextCtrl->ChangeValue(to_string(affine.yy));
}

void AevusFrame::onExit(wxCommandEvent& event) {
    Close(true);
}

void AevusFrame::onAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets' Hello world sample",
        "About Hello World", wxOK | wxICON_INFORMATION);
}

void AevusFrame::onFileOpen(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open flame file", "", "",
        "Flame files (*.flame)|*.flame|XML files|*.xml",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    auto filename = openFileDialog.GetPath().ToStdString();
    FILE* inputStream = fopen(filename.c_str(), "r");
    if (inputStream == NULL) {
        printf("Error on opening file: %s\n", filename.c_str());
        return;
    }
    flame->deserialize(inputStream);
    fclose(inputStream);
    wxCommandEvent flameUpdateEvent(FLAME_UPDATE_EVENT, wxID_ANY);
    flameUpdateEvent.SetEventObject(this);
    ProcessWindowEvent(flameUpdateEvent);
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