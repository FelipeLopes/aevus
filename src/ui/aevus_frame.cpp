#include "aevus_frame.hpp"
#include "wxfb/code/wxfb_frame.h"

namespace ui {

AevusFrame::AevusFrame(wxWindow* parent): WxfbFrame(parent) {
    SetStatusText("Welcome to wxWidgets!");
}

void AevusFrame::onExit(wxCommandEvent& event) {
    Close(true);
}

void AevusFrame::onAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets' Hello world sample",
        "About Hello World", wxOK | wxICON_INFORMATION);
}

void AevusFrame::onHello(wxCommandEvent& event) {
    wxLogMessage("Hello world from wxWidgets!");
}

AevusFrame::~AevusFrame() { }

bool MyApp::OnInit()
{
    AevusFrame* frame = new AevusFrame(NULL);
    frame->Show( true );
    return true;
}

}