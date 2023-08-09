#include "aevus_frame.hpp"
#include "wxfb/code/wxfb_frame.h"
#include <memory>
#include <string>

using std::to_string;

namespace ui {

wxDEFINE_EVENT(FLAME_UPDATE_EVENT, wxCommandEvent);

AevusFrame::AevusFrame(std::shared_ptr<core::Flame> flame_): WxfbFrame(NULL),
    flame(flame_)
{
    SetStatusText("Welcome to Aevus!");
    Bind(FLAME_UPDATE_EVENT, &AevusFrame::onFlameUpdate, this, ID_FLAME_UPDATE);
}

void AevusFrame::onTextFlameUpdate(wxCommandEvent& event) {
    printf("Text flame update\n");
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
    wxCommandEvent flameUpdateEvent(FLAME_UPDATE_EVENT, ID_FLAME_UPDATE);
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