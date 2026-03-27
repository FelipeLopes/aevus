#include "palette_frame.hpp"
#include "aevus_frame.hpp"
#include "gradient_model.hpp"
#include <wx/string.h>
#include <wx/dataview.h>
#include <wx/persist/toplevel.h>
#include <wx/config.h>

namespace ui {

PaletteFrame::PaletteFrame(wxWindow* parent, const wxPoint& pos, const wxSize& size):
    wxFrame(parent, wxID_ANY, "Palette Editor", pos, size)
{
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PaletteFrame::onClose ) );
    wxPersistentRegisterAndRestore(this, "palette_editor");
    wxBoxSizer* paletteSizer = new wxBoxSizer(wxHORIZONTAL);
    wxDataViewCtrl* gradientDataViewCtrl = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
    paletteSizer->Add(gradientDataViewCtrl, 1, wxALL | wxEXPAND, 5);

    SetSizer(paletteSizer);

    gradientModel = new GradientModel();

    gradientDataViewCtrl->AssociateModel(gradientModel.get());
    gradientDataViewCtrl->AppendColumn(
        new wxDataViewColumn(
            "title",
            new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_EDITABLE),
            0,
            FromDIP(200),
            wxALIGN_LEFT,
            wxDATAVIEW_COL_RESIZABLE
        )
    );
    gradientDataViewCtrl->AppendColumn(
        new wxDataViewColumn(
            "value",
            new wxDataViewTextRenderer("long", wxDATAVIEW_CELL_EDITABLE ),
            1,
            FromDIP(100),
            wxALIGN_LEFT,
            wxDATAVIEW_COL_RESIZABLE
        )
    );
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