#pragma once

#include <wx/panel.h>

namespace ui {

class GradientController {
public:
    GradientController(wxPanel* gradientPanel);
    void handlePaint();
private:
    wxPanel* gradientPanel;
};

}