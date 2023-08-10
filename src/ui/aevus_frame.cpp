#include "aevus_frame.hpp"
#include "wxfb/code/wxfb_frame.h"
#include <memory>
#include <stdexcept>
#include <string>

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

void AevusFrame::onEnterFlameUpdate(wxCommandEvent& event) {
    int enterId = event.GetId();
    if (!flameTextEqual(enterId)) {
        printf("Enter flame update\n");
    }
}

void AevusFrame::onFocusFlameEdit(wxFocusEvent& event) {
    int focusId = event.GetId();
    if (editingId != focusId && editingId != -1) {
        if (!flameTextEqual(editingId)) {
            printf("Focus flame update\n");
        }
    }
    editingId = focusId;
}

bool AevusFrame::flameTextEqual(int textCtrlId) {
    std::string text;
    switch (textCtrlId) {
        case ID_FLAME_PRE_OX: text = preOXtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_OY: text = preOYtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_XX: text = preXXtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_XY: text = preXYtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_YX: text = preYXtextCtrl->GetValue(); break;
        case ID_FLAME_PRE_YY: text = preYYtextCtrl->GetValue(); break;
        default: throw std::invalid_argument("Unrecognized ID");
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
        default: throw std::invalid_argument("Unrecognized ID");
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