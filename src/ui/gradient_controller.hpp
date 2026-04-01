#pragma once

#include "../core/content.hpp"
#include "../core/gradient.hpp"
#include <wx/panel.h>

namespace ui {

class GradientController {
public:
    GradientController(wxPanel* gradientPanel, std::optional<core::ColormapContent> content);
    void handlePaint();
    void handleFlameContent(std::optional<core::FlameContent> content);
    void handleColormapContent(core::ColormapContent content);
    void handleMouseDown(wxMouseEvent& event);
    std::optional<core::ColormapContent> getContent();
private:
    static std::string getThumbSvgStringForColor(core::GradientColor color, bool selected);
    std::optional<core::ColormapContent> content;
    wxPanel* gradientPanel;
    int selectedStop = -1;
};

}