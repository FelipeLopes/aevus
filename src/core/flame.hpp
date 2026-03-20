#pragma once

#include <cstdint>
#include <boost/bimap.hpp>
#include <optional>
#include "color.hpp"
#include "custom_coefs.hpp"
#include "palette.hpp"
#include "xform.hpp"
#include "serializable.hpp"

namespace core {

struct FrameCL {
    float cx, cy, scale;
    int width, height;
    FrameCL() {
        cx = cy = 0;
        scale = 100;
        width = 732;
        height = 640;
    }
};

struct IterationState {
    float x, y, c;
    uint8_t xf;
    union SeedUnion {
        struct Seed {
            uint32_t low, hi;
        } word;
        uint64_t value;
    } seed;
};

enum ClippingMode {
    ALPHA,
    CHANNEL,
    WHITE
};

class Clipping {
public:
    Clipping();
    ClippingMode mode;
    std::optional<std::string> toString();
    void fromString(std::optional<std::string> text);
};

class FlameV: public SerializableV {
public:
    FlameV();
    std::string version, name;
    SizeParams size;
    CenterParams center;
    double scale, quality;
    Color background;
    double brightness, contrast, gamma, vibrancy;
    ClippingMode clippingMode;
    std::vector<XFormV> xforms;
    std::optional<FinalXFormV> finalXForm;
    PaletteV palette;
    virtual void acceptSerializer(Serializer& serializer);
    virtual void acceptDeserializer(Deserializer& deserializer);
};

}