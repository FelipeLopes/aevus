#include "palette_frame.hpp"
#include "aevus_frame.hpp"

namespace ui {

PaletteFrame::PaletteFrame(wxWindow* parent, const wxPoint& pos, const wxSize& size):
    wxFrame(parent, wxID_ANY, "Gradient Editor", pos, size)
{
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PaletteFrame::onClose ) );
}

PaletteFrame::~PaletteFrame() {
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PaletteFrame::onClose ) );
}

void PaletteFrame::onClose(wxCloseEvent& event) {
    dynamic_cast<AevusFrame*>(GetParent())->onPaletteEditorClosed();
    Show(false);
}

}