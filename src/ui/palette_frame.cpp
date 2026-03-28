#include "palette_frame.hpp"
#include "aevus_frame.hpp"
#include "gradient_model.hpp"
#include <wx-3.2/wx/gdicmn.h>
#include <wx/string.h>
#include <wx/dataview.h>
#include <wx/persist/toplevel.h>
#include <wx/config.h>

namespace ui {

PaletteFrame::PaletteFrame(wxWindow* parent, core::PresetLibrary* presetLibrary):
    wxFrame(parent, wxID_ANY, "Palette Editor", wxDefaultPosition, wxDefaultSize)
{
    this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PaletteFrame::onClose ) );
    wxPersistentRegisterAndRestore(this, "palette_editor");
    wxBoxSizer* paletteSizer = new wxBoxSizer(wxHORIZONTAL);
    auto gradientDataViewCtrl = new GradientDataViewCtrl(this);
    paletteSizer->Add(gradientDataViewCtrl, 1, wxALL | wxEXPAND, 5);

    SetSizer(paletteSizer);

    gradientModel = new GradientModel(presetLibrary);

    gradientDataViewCtrl->AssociateModel(gradientModel.get());
    gradientDataViewCtrl->AppendColumn(
        new wxDataViewColumn(
            "title",
            new wxDataViewIconTextRenderer("wxDataViewIconText", wxDATAVIEW_CELL_EDITABLE),
            0,
            FromDIP(200),
            wxALIGN_LEFT,
            wxDATAVIEW_COL_RESIZABLE
        )
    );
    gradientDataViewCtrl->AppendColumn(
        new wxDataViewColumn(
            "value",
            new wxDataViewBitmapRenderer("wxBitmapBundle", wxDATAVIEW_CELL_EDITABLE ),
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