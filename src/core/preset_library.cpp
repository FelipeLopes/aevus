#include "preset_library.hpp"
namespace core {

using grad::Grd5SolidGradient;

PresetLibrary::PresetLibrary() { }

bool PresetLibrary::addGrd5Gradient(const Grd5SolidGradient& grd5Gradient) {
    if (!grd5Gradient.canBeImported()) {
        return false;
    }
    gradients.emplace_back(grd5Gradient);
    return true;
}

}