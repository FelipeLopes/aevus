#pragma once

#include <wx/panel.h>

namespace ui {

class FlameModel {
public:
    FlameModel(wxPanel* flamePanel);
    void handlePaint();
    void setBitmap(const wxBitmap& bitmap);
private:
    wxPanel* flamePanel;
    wxBitmap flameBitmap;
};

}