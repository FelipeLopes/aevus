#pragma once

#include "gradient_model.hpp"
#include <wx/frame.h>

namespace ui {

class PaletteFrame: public wxFrame {
public:
    PaletteFrame(wxWindow* parent, core::PresetLibrary* presetLibrary);
    ~PaletteFrame();
private:
    void onClose(wxCloseEvent& event);
    wxObjectDataPtr<GradientModel> gradientModel;
};

}