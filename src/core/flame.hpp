#pragma once

#include <cstdint>
#include <boost/bimap.hpp>
#include <optional>
#include "../serial/xml_element.hpp"
#include "color.hpp"
#include "palette.hpp"
#include "xform.hpp"

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

class Clipping : public serial::StringSerializable {
public:
    Clipping();
    ClippingMode mode;
    std::optional<std::string> toString() override;
    void fromString(std::optional<std::string> text) override;
};

class Flame: public serial::XMLElementClass {
public:
    Flame();
    serial::XMLAttributeString version;
    serial::XMLAttributeString name;
    serial::XMLAttribute<SizeParams> size;
    serial::XMLAttribute<CenterParams> center;
    serial::XMLAttributeDouble scale;
    serial::XMLAttributeDouble quality;
    serial::XMLAttribute<Color> background;
    serial::XMLAttributeDouble brightness;
    serial::XMLAttributeDouble contrast;
    serial::XMLAttributeDouble gamma;
    serial::XMLAttributeDouble vibrancy;
    serial::XMLAttribute<Clipping> clipping;
    serial::ListXMLElementClass<XForm> xforms;
    serial::OptionalXMLElementClass<FinalXForm> finalXForm;
    Palette palette;

    int readFinalXFormPosition() const;

private:
    tinyxml2::XMLNode* nodeDeserialize(tinyxml2::XMLNode* node) override;
};

}