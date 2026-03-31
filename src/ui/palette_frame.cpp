#include "palette_frame.hpp"
#include "aevus_frame.hpp"
#include "flame_view.hpp"
#include "gradient_controller.hpp"
#include "gradient_model.hpp"
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/dataview.h>
#include <wx/persist/toplevel.h>
#include <wx/config.h>

namespace ui {

PaletteFrameBase::PaletteFrameBase(wxWindow* parent):
    wxFrame(parent, wxID_ANY, "Palette Editor", wxDefaultPosition, wxDefaultSize)
{
    wxPersistentRegisterAndRestore(this, "palette_editor");
    wxBoxSizer* paletteSizer = new wxBoxSizer(wxVERTICAL);
    gradientDataViewCtrl = new GradientDataViewCtrl(this);
    paletteSizer->Add(gradientDataViewCtrl, 1, wxALL | wxEXPAND, 5);

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
    bitmapColumn = new wxDataViewColumn(
        "value",
        new wxDataViewBitmapRenderer("wxBitmapBundle", wxDATAVIEW_CELL_EDITABLE ),
        1,
        FromDIP(100),
        wxALIGN_LEFT,
        wxDATAVIEW_COL_RESIZABLE
    );
    gradientDataViewCtrl->AppendColumn(bitmapColumn);

    gradientPanel = new wxPanel(this, ID_GRADIENT_PANEL, wxDefaultPosition, wxSize(-1, 100));
    paletteSizer->Add(gradientPanel, 0, wxEXPAND | wxALL, 5);

    SetSizer(paletteSizer);

    Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(PaletteFrameBase::onClose));
    gradientPanel->Connect(wxEVT_PAINT, wxPaintEventHandler(PaletteFrameBase::onPaint), NULL, this);
}

PaletteFrameBase::~PaletteFrameBase() {
    this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(PaletteFrameBase::onClose));
    gradientPanel->Disconnect(wxEVT_PAINT, wxPaintEventHandler(PaletteFrameBase::onPaint), NULL, this);
}

PaletteFrame::PaletteFrame(wxWindow* parent, FlameView* flameView, core::PresetLibrary* presetLibrary):
    PaletteFrameBase(parent),
    gradientModel(new GradientModel(presetLibrary, bitmapColumn)),
    gradientController(gradientPanel, flameView == NULL ? (std::optional<core::ColormapContent>)std::nullopt : flameView->getColormapContent())
{
    setupFlameView(flameView);
    gradientDataViewCtrl->AssociateModel(gradientModel.get());
    gradientDataViewCtrl->Connect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED,
        wxDataViewEventHandler( PaletteFrame::onPresetsSelectionChanged ), NULL, this );
    gradientPanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

PaletteFrame::~PaletteFrame() {
    removeSignalConnections();
    gradientDataViewCtrl->Disconnect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED,
        wxDataViewEventHandler( PaletteFrame::onPresetsSelectionChanged ), NULL, this );
}

void PaletteFrame::setupFlameView(FlameView* flameView_) {
    removeSignalConnections();
    flameView = flameView_;
    if (flameView != NULL) {
        connections.push_back(flameView->flameContent.connect(
            bind(&GradientController::handleFlameContent, &gradientController, _1)
        ));
        connections.push_back(flameView->colormapContent.connect(
            bind(&GradientController::handleColormapContent, &gradientController, _1)
        ));
        connections.push_back(gradientUpdate.connect(
            bind(&FlameView::handleGradientContent, flameView, _1)
        ));
    }
}

void PaletteFrame::onPresetsSelectionChanged(wxDataViewEvent& event) {
    auto item = event.GetItem();
    if (item.IsOk()) {
        auto itemPtr = static_cast<GradientModelNode*>(item.GetID());
        if (auto leafNode = dynamic_cast<GradientLeafNode*>(itemPtr)) {
            gradientUpdate(*leafNode->gradient);
        }
    }
}

void PaletteFrame::onClose(wxCloseEvent& event) {
    dynamic_cast<AevusFrame*>(GetParent())->onPaletteEditorClosed();
    wxPersistenceManager::Get().SaveAndUnregister(this);
    wxConfig::Get()->Flush();
    event.Skip();
}

void PaletteFrame::onPaint(wxPaintEvent& event) {
    if (event.GetId() == ID_GRADIENT_PANEL) {
        gradientController.handlePaint();
    }
}

void PaletteFrame::removeSignalConnections() {
    for (int i=0; i<connections.size(); i++) {
        connections[i].disconnect();
    }
    connections.clear();
}

}