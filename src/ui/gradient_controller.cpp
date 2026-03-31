#include "gradient_controller.hpp"
#include <cmath>
#include <optional>
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

using std::to_string;
using std::vector;

namespace ui {

GradientController::GradientController(wxPanel* gradientPanel_, std::optional<core::Gradient> flameGradient_):
    content(flameGradient_), gradientPanel(gradientPanel_) { }

void GradientController::handlePaint() {
    wxAutoBufferedPaintDC dc(gradientPanel);
    dc.Clear();
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    int padding = 10;
    int height = (gradientPanel->GetSize().GetHeight()-2*padding)/2;
    int width = std::max(300, gradientPanel->GetSize().GetWidth()-2*padding);
    int thumbWidth = 30;
    int thumbHeight = 30;
    if (gc) {
        if (content.has_value()) {
            vector<uint8_t> imageBytes;
            content->renderPNG(width, height, imageBytes);
            auto bitmap = wxBitmap::NewFromPNGData(imageBytes.data(), imageBytes.size());
            gc->DrawBitmap(bitmap, padding, padding, width, height);
            for (int i=0; i<content->colorStops.size(); i++) {
                auto stop = content->colorStops[i];
                auto svg = getThumbSvgStringForColor(stop.color, i == selectedStop);
                core::SvgDocument::renderStringToPNG(svg.c_str(), thumbWidth, thumbHeight, imageBytes);
                bitmap = wxBitmap::NewFromPNGData(imageBytes.data(), imageBytes.size());
                double offsetX = width*stop.location - 0.5*thumbWidth;
                double offsetY = height - 0.45*thumbHeight;
                gc->DrawBitmap(bitmap, padding+offsetX, padding+offsetY, thumbWidth, thumbHeight);
            }
        }
        delete gc;
    }
}

std::string GradientController::getThumbSvgStringForColor(core::GradientColor color, bool selected) {
    auto templateStr = std::string(
        "<svg width=\"100\" height=\"100\" viewBox=\"0 0 100 100\" xmlns=\"http://www.w3.org/2000/svg\"> \
            <defs> \
                <polygon id=\"polygon\" points=\"25 30, 25 95, 75 95, 75 30, 50 5\" /> \
            </defs> \
            <use href=\"#polygon\" fill=\"FILLCOLOR\" stroke=\"black\" stroke-width=\"6\" /> \
            <use href=\"#polygon\" fill=\"none\" stroke=\"white\" stroke-width=\"3\" />") +
             (selected ? "<circle cx=\"50\" cy=\"50\" r=\"10\" fill=\"black\" stroke=\"white\" stroke-width=\"3\"/>" : "") +
        "</svg>";
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

void GradientController::handleFlameContent(std::optional<core::FlameContent> flameContent) {
    if (flameContent.has_value()) {
        content = flameContent->gradient;
    } else {
        content = std::nullopt;
    }
    gradientPanel->Refresh();
}

void GradientController::handleGradientContent(core::Gradient gradientContent) {
    content = gradientContent;
    gradientPanel->Refresh();
}

}