#include "preset_library.hpp"
#include "color.hpp"

namespace core {

using grad::Grd5SolidGradient;

PresetLibrary::PresetLibrary() {
    // Currently, these background and foreground colors are only used to convert
    // Photoshop gradients, which might have colors that depend on bg or fg.
    // We are leaving these variables hard coded in case bg or fg dependent colors
    // become useful later.
    bgColor = Color(0, 0, 0);
    fgColor = Color(255, 255, 255);
}

bool PresetLibrary::addGrd5Gradient(const Grd5SolidGradient& grd5Gradient) {
    if (grd5Gradient.usesBookColor()) {
        return false;
    }
    gradients.emplace_back(grd5Gradient);
    return true;
}

}