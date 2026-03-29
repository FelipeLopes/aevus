#pragma once

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
    PaletteFrame(wxWindow* parent, core::PresetLibrary* presetLibrary);
private:
    wxObjectDataPtr<GradientModel> gradientModel;
    GradientController gradientController;
    void onClose(wxCloseEvent& event) override;
    void onPaint(wxPaintEvent& event) override;
};

}