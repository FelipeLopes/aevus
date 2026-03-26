#include "palette_frame.hpp"
#include "aevus_frame.hpp"
#include <wx/persist/toplevel.h>
#include <wx/config.h>

namespace ui {

PaletteFrame::PaletteFrame(wxWindow* parent, const wxPoint& pos, const wxSize& size):
    wxFrame(parent, wxID_ANY, "Palette Editor", pos, size)
{
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PaletteFrame::onClose ) );
    if( !wxPersistentRegisterAndRestore(this, "palette_editor")) {
        printf("register/restore failed\n");
    }
}

PaletteFrame::~PaletteFrame() {
    this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PaletteFrame::onClose ) );
}

void PaletteFrame::onClose(wxCloseEvent& event) {
    dynamic_cast<AevusFrame*>(GetParent())->onPaletteEditorClosed();
    wxPersistenceManager::Get().SaveAndUnregister(this);
    wxConfig::Get()->Flush();
    event.Skip();
}

}