#pragma once

#include "xml_serialization.hpp"
#include <cstdint>
#include <boost/bimap.hpp>
#include <optional>

namespace core {

struct ColorCL {
    float r,g,b,a;
};

class PaletteColors: public StringSerializable {
public:
    PaletteColors();
    std::string hexAt(int pos) const;
    void readColorCLArray(std::vector<ColorCL>& arr) const;
    virtual std::optional<std::string> toString();
    virtual void fromString(std::optional<std::string> text);
    static const unsigned PALETTE_WIDTH = 256;
private:
    typedef unsigned char byte;
    char hexHigh(byte b) const;
    char hexLow(byte b) const;
    byte hexValue(char high, char low) const;
    std::vector<byte> paletteData;
};

class Palette: public XMLElementClass {
public:
    Palette(XMLElementClass& el);
    XMLAttributeInt count;
    XMLAttributeString format;
    XMLContent<PaletteColors> colors;
    void readColorCLArray(std::vector<ColorCL>& arr) const;
};

class SizeParams: public StringSerializable {
public:
    SizeParams();
    SizeParams(int width, int height);
    int width, height;
    virtual std::optional<std::string> toString();
    virtual void fromString(std::optional<std::string> text);
};

class CenterParams: public StringSerializable {
public:
    CenterParams();
    CenterParams(double x, double y);
    double x, y;
    virtual std::optional<std::string> toString();
    virtual void fromString(std::optional<std::string> text);
};

struct Variation {
    enum VariationID {
        NO_VARIATION = -1,
        LINEAR = 0,
        SPHERICAL = 2,
        POLAR = 5,
        HYPERBOLIC = 10,
        DIAMOND = 11,
        EYEFISH = 27,
        CYLINDER = 29,
        SQUARE = 43
    };

    const static boost::bimap<VariationID, std::string> variationNames;
};

class VariationMap: public StringMapSerializable {
public:
    VariationMap();
    virtual std::map<std::string, std::string> toStringMap();
    virtual void fromStringMap(std::map<std::string, std::string> stringMap);
    std::map<Variation::VariationID, double> variations;
};

class Affine: public StringSerializable {
public:
    Affine(bool serializeIdentity);
    virtual std::optional<std::string> toString();
    virtual void fromString(std::optional<std::string> text);
    double xx, xy, yx, yy, ox, oy;
private:
    bool serializeIdentity;
};

class CoefsAffine: public Affine {
public:
    CoefsAffine();
};

class PostAffine: public Affine {
public:
    PostAffine();
};

class Chaos: public StringSerializable {
public:
    Chaos();
    virtual std::optional<std::string> toString();
    virtual void fromString(std::optional<std::string> text);
    double getXaos(int idx);
private:
    std::vector<double> chaos;
};

class ColorSpeed: public StringMapSerializable {
public:
    ColorSpeed();
    virtual std::map<std::string, std::string> toStringMap();
    virtual void fromStringMap(std::map<std::string, std::string> stringMap);
private:
    double colorSpeed;
};

struct XFormCL {
    static const int MAX_VARIATIONS = 10;
    struct VariationData {
        Variation::VariationID id;
        float weight;
    };
    VariationData varData[MAX_VARIATIONS];
    float a, b, c, d, e, f;
    float pa, pb, pc, pd, pe, pf;
};

class XForm: public XMLElementClass {
public:
    XForm();
    XMLAttributeDouble weight;
    XMLAttributeDouble color;
    XMLMultiAttribute<VariationMap> variationMap;
    XMLAttribute<CoefsAffine> coefs;
    XMLAttribute<PostAffine> post;
    XMLAttribute<Chaos> chaos;
    XMLMultiAttribute<ColorSpeed> colorSpeed;
    XFormCL toXFormCL() const;
};

class Color: public StringSerializable {
public:
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);
    uint8_t r, g, b;
    virtual std::optional<std::string> toString();
    virtual void fromString(std::optional<std::string> text);
};

struct FlameCL {
    float cx, cy, scale;
    int width, height;
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

class Flame: public XMLElementClass {
public:
    Flame();
    XMLAttributeString version;
    XMLAttributeString name;
    XMLAttribute<SizeParams> size;
    XMLAttribute<CenterParams> center;
    XMLAttributeDouble scale;
    XMLAttributeInt quality;
    XMLAttribute<Color> background;
    XMLAttributeDouble brightness;
    XMLAttributeDouble contrast;
    ListXMLElementClass<XForm> xforms;
    Palette palette;

    FlameCL getFlameCL() const;
    void readInitialStateArray(std::vector<IterationState>& arr, int size) const;
    void readXFormCLArray(std::vector<XFormCL>& arr) const;
    void readXFormDistribution(std::vector<uint8_t>& dist) const;

    const static int XFORM_DISTRIBUTION_GRAINS = 16384;
};

}