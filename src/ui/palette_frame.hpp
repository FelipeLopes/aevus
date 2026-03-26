#pragma once

#include <wx/frame.h>

namespace ui {

class PaletteFrame: public wxFrame {
public:
    PaletteFrame(wxWindow* parent, const wxPoint& pos, const wxSize& size);
    ~PaletteFrame();
private:
    void onClose(wxCloseEvent& event);
};

}