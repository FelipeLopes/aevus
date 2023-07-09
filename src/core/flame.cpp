#include "flame.hpp"
#include "xml_serialization.hpp"

namespace core {

Flame::Flame(): XMLElementClass("flame"),
    version(*this, "version"),
    scale(*this, "scale"),
    quality(*this, "quality"),
    brightness(*this, "brightness"),
    contrast(*this, "contrast"),
    palette(*this) { }

Palette::Palette(XMLElementClass& el): XMLElementClass(el, "palette"),
    count(*this, "count"),
    format(*this, "format"),
    colors(*this) { }

}