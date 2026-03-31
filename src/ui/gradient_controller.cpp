#include "gradient_controller.hpp"
#include <cmath>
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

using std::to_string;

namespace ui {

GradientController::GradientController(wxPanel* gradientPanel_, std::optional<core::Gradient> flameGradient_):
    flameGradient(flameGradient_), gradientPanel(gradientPanel_) { }

void GradientController::handlePaint() {
    wxAutoBufferedPaintDC dc(gradientPanel);
    dc.Clear();
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc) {
        delete gc;
    }
}

std::string GradientController::getThumbSvgStringForColor(core::GradientColor color) {
    std::string templateStr =
        "<svg width=\"100\" height=\"100\" viewBox=\"0 0 100 100\" xmlns=\"http://www.w3.org/2000/svg\"> \
            <defs> \
                <polygon id=\"polygon\" points=\"25 30, 25 95, 75 95, 75 30, 50 5\" /> \
            </defs> \
            <use href=\"#polygon\" fill=\"FILLCOLOR\" stroke=\"black\" stroke-width=\"4\" /> \
            <use href=\"#polygon\" fill=\"none\" stroke=\"white\" stroke-width=\"2\" /> \
        </svg>";
    std::string token = "FILLCOLOR";
    uint8_t r, g, b;
    r = lround(color.r * 255.0);
    g = lround(color.g * 255.0);
    b = lround(color.b * 255.0);
    auto colorString = std::string("rgb(") + to_string(r) + "," + to_string(g) + "," + to_string(b) + ")";
    auto it = templateStr.find(token);
    templateStr.replace(it, token.length(), colorString);
    return templateStr;
}

void GradientController::handleFlameContent(std::optional<core::FlameContent> content) {
    if (content.has_value()) {
        printf("gradient controller: %zu\n", content.value().gradient.colorStops.size());
    }
}

}