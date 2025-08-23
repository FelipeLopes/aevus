#include "flame.hpp"
#include <memory>
#include <random>
#include <stdexcept>

using std::string;

namespace core {

Clipping::Clipping(): mode(WHITE) { }

std::optional<std::string> Clipping::toString() {
    switch(mode) {
        case ALPHA: return "alpha";
        case CHANNEL: return "channel";
        case WHITE: return "white";
    }
}

void Clipping::fromString(std::optional<std::string> text) {
    if (!text.has_value()) {
        mode = WHITE;
    } else if (text == "alpha") {
        mode = ALPHA;
    } else if (text == "channel") {
        mode = CHANNEL;
    } else if (text == "white") {
        mode = WHITE;
    } else {
        throw std::invalid_argument("Unrecognized clipping mode");
    }
}

Flame::Flame(): XMLElementClass("flame"),
    version(*this, "version"),
    name(*this, "name"),
    size(*this, "size"),
    center(*this, "center"),
    scale(*this, "scale"),
    quality(*this, "quality"),
    background(*this, "background"),
    brightness(*this, "brightness"),
    contrast(*this, "contrast", 1.0),
    gamma(*this, "gamma"),
    vibrancy(*this, "vibrancy", 1.0),
    clipping(*this, "clipping"),
    xforms(*this, "xform"),
    finalXForm(*this, "finalxform"),
    palette(*this)
{
    center.get()->x = 0;
    center.get()->y = 0;
    scale.setValue(100);
    size.get()->width = 732;
    size.get()->height = 640;
    quality.setValue(5);
    brightness.setValue(4);
    contrast.setValue(1);
    gamma.setValue(4);
    vibrancy.setValue(1);
}

tinyxml2::XMLNode* Flame::nodeDeserialize(tinyxml2::XMLNode* node) {
    auto element = node->ToElement();
    if (element != NULL) {
        string name = element->Name();
        if (name == "flames") {
            return serial::XMLElementClass::nodeDeserialize(node->FirstChild());
        }
    }
    return serial::XMLElementClass::nodeDeserialize(node);
}

}