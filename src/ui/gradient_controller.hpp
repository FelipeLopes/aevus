#pragma once

#include "../core/content.hpp"
#include "../core/gradient.hpp"
#include <wx/panel.h>

namespace ui {

class GradientController {
public:
    GradientController(wxPanel* gradientPanel, std::optional<core::Gradient> flameGradient);
    void handlePaint();
    void handleFlameContent(std::optional<core::FlameContent> content);
    void handleMouseDown(wxMouseEvent& event);
private:
    static std::string getThumbSvgStringForColor(core::GradientColor color, bool selected);
    std::optional<core::Gradient> content;
    wxPanel* gradientPanel;
    int selectedStop = -1;
};

}