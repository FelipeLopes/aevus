#pragma once

#include "flame_view.hpp"
#include "gradient_controller.hpp"
#include "gradient_model.hpp"
#include <wx/panel.h>
#include <wx/frame.h>

namespace ui {

class PaletteFrameBase: public wxFrame {
public:
    PaletteFrameBase(wxWindow* parent);
    ~PaletteFrameBase();
protected:
    enum {
        ID_GRADIENT_PANEL = 7000
    };
    wxPanel* gradientPanel;
    wxDataViewCtrl* gradientDataViewCtrl;
    wxDataViewColumn* bitmapColumn;
    virtual void onClose(wxCloseEvent& event) { event.Skip(); };
    virtual void onPaint(wxPaintEvent& event) { event.Skip(); };
};

class PaletteFrame: public PaletteFrameBase {
public:
    PaletteFrame(wxWindow* parent, FlameView* flameView, core::PresetLibrary* presetLibrary);
    ~PaletteFrame();
    void setupFlameView(FlameView* flameView);
private:
    FlameView* flameView = NULL;
    wxObjectDataPtr<GradientModel> gradientModel;
    GradientController gradientController;
    void removeSignalConnections();
    std::vector<boost::signals2::connection> connections;
    void onClose(wxCloseEvent& event) override;
    void onPaint(wxPaintEvent& event) override;
};

}