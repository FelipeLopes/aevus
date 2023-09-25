#pragma once

#include <cstdint>
#include <boost/bimap.hpp>
#include <optional>
#include "../serial/xml_element.hpp"
#include "color.hpp"
#include "palette.hpp"
#include "xform.hpp"

namespace core {

struct FlameCL {
    float cx, cy, scale;
    int width, height;
    FlameCL() {
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
    serial::XMLAttributeInt quality;
    serial::XMLAttribute<Color> background;
    serial::XMLAttributeDouble brightness;
    serial::XMLAttributeDouble contrast;
    serial::XMLAttribute<Clipping> clipping;
    serial::ListXMLElementClass<XForm> xforms;
    Palette palette;

    FlameCL getFlameCL() const;
    void readInitialStateArray(std::vector<IterationState>& arr, int size) const;
    void readXFormData(std::vector<XFormCL>& xformVec, std::vector<VariationCL>& varVec,
        std::vector<float>& paramVec) const;
    void readXFormDistribution(std::vector<uint8_t>& dist) const;

    const static int XFORM_DISTRIBUTION_GRAINS = 16384;

private:
    tinyxml2::XMLNode* nodeDeserialize(tinyxml2::XMLNode* node) override;
};

}