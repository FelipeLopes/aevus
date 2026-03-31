#include "flame.hpp"
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

Flame::Flame() {
    center.x = 0;
    center.y = 0;
    scale = 100;
    size.width = 732;
    size.height = 640;
    quality = 5;
    brightness = 4;
    contrast = 1;
    gamma = 4;
    vibrancy = 1;
    vectorWeight = 0;
}

void Flame::acceptSerializer(Serializer& serializer) {
    serializer.serialize(*this);
}

void Flame::acceptDeserializer(Deserializer& deserializer) {
    deserializer.deserialize(*this);
}


}