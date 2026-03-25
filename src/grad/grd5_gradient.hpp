#pragma once

#include <cstdint>
#include <cstdio>
#include <inttypes.h>
#include <memory>
#include <string>
#include <map>
#include <vector>

namespace grad {

enum Grd5Type {
    TYPE_UNKNOWN,
    TYPE_PATTERN,
    TYPE_DESCRIPTION,
    TYPE_VAR_LEN_LIST,
    TYPE_TEXT,
    TYPE_OBJECT,
    TYPE_UNTF,
    TYPE_BOOL,
    TYPE_LONG,
    TYPE_DOUBLE,
    TYPE_ENUM,
    TYPE_TDTA
};

class Grd5String {
public:
    std::vector<char> content;
};

class Grd5TypeNameString: public Grd5String {
public:
    std::string toString() const;
};

class Grd5Ucs2String: public Grd5String {
public:
    std::string toString() const;
};

class Grd5TdtaString: public Grd5String {
public:
    std::string toString() const;
};

class Grd5Extremum {
public:
    int type;
    union {
        double d;
        uint32_t u;
    };
};

class Grd5Extrema {
public:
    std::vector<Grd5Extremum> extremumList;
};

class Grd5OpacityStop { 
public:
    double Opct;
    uint32_t Lctn, Mdpn;
};

class Grd5Color {
public:
    virtual ~Grd5Color() = default;
};

class Grd5RgbColor: public Grd5Color {
public:
    Grd5RgbColor();
    Grd5RgbColor(double r, double g, double b);
    double Rd, Grn, Bl;
};

class Grd5HsvColor: public Grd5Color {
public:
    Grd5HsvColor();
    double H, Strt, Brgh;
};

class Grd5LabColor: public Grd5Color {
public:
    Grd5LabColor();
    double Lmnc, A, B;
};

class Grd5CmykColor: public Grd5Color {
public:
    Grd5CmykColor();
    double Cyn, Mgnt, Ylw, Blck;
};

class Grd5GrayScaleColor: public Grd5Color {
public:
    Grd5GrayScaleColor(double val);
    double val;
};

class Grd5ForegroundColor: public Grd5Color {

};

class Grd5BackgroundColor: public Grd5Color {

};

class Grd5BookColor: public Grd5Color {
public:
    Grd5BookColor(Grd5Ucs2String Bk, Grd5Ucs2String Nm, uint32_t bookId, Grd5TdtaString bookKey);
    Grd5Ucs2String Bk, Nm;
    uint32_t bookId;
    Grd5TdtaString bookKey;
};

class Grd5UnspecifiedColor: public Grd5Color { };

class Grd5ColorStop {
public:
    std::shared_ptr<Grd5Color> color;
    uint32_t Lctn, Mdpn;
};


class Grd5Gradient {
public:
    Grd5Ucs2String title;
    virtual ~Grd5Gradient() = default;
};

class Grd5SolidGradient: public Grd5Gradient {
public:
    double smoothness;
    std::vector<Grd5OpacityStop> opacityStops;
    std::vector<Grd5ColorStop> colorStops;
    bool canBeImported() const;
};

enum Grd5ColorModelType {
    COLOR_MODEL_UNKNOWN,
    COLOR_MODEL_RGB,
    COLOR_MODEL_HSV,
    COLOR_MODEL_LAB,
    COLOR_MODEL_CMYK,
    COLOR_MODEL_GRAYSCALE,
    COLOR_MODEL_FOREGROUND,
    COLOR_MODEL_BACKGROUND,
    COLOR_MODEL_BOOK,
    COLOR_MODEL_UNSPECIFIED
};

class Grd5NoiseGradient: public Grd5Gradient {
public:
    bool showTransparency, vectorColor;
    uint32_t seed, smoothness;
    Grd5ColorModelType model;
    Grd5Extrema min, max;
};

class Grd5GradientList {
public:
    std::vector<std::shared_ptr<Grd5Gradient>> gradients;
};

class Grd5Object {
public:
    Grd5Ucs2String displayName;
    Grd5TypeNameString typeName;
    uint32_t value;
};

class Grd5Enum {
public:
    Grd5TypeNameString name, subname;
};

class Grd5Stream {
public:
    Grd5Stream(const char* filename);
    ~Grd5Stream();
    Grd5GradientList readGradientList();
private:

    enum Grd5StringType {
        STRING_TYPENAME,
        STRING_UCS2,
        STRING_TDTA
    };

    enum Grd5GradientType {
        GRADIENT_SOLID,
        GRADIENT_NOISE,
        GRADIENT_UNKNOWN
    };

    void raiseFileStreamError();
    void raiseTypeNameMismatch();
    void raiseTypeIdMismatch();
    void raiseComponentMismatch();
    void readBytes(uint32_t len, char* arr);
    uint16_t readUint16();
    uint32_t readUint32();
    double readDouble();
    bool readBool();
    Grd5Type readType();
    void readString(Grd5StringType type, uint32_t len, Grd5String& str);
    Grd5TypeNameString readTypeNameString();
    Grd5Ucs2String readUcs2String();
    Grd5TdtaString readTdtaString();
    void parseNamedType(std::string expectedName, Grd5Type expectedType);
    uint32_t readVllLength(std::string expectedName);
    double readNamedDouble(std::string expectedName);
    double readUnitDouble(std::string expectedName, std::string expectedUnit);
    std::shared_ptr<Grd5Gradient> readGradient();
    Grd5GradientType readGradientType();
    std::shared_ptr<Grd5SolidGradient> readSolidGradient();
    Grd5ColorStop readColorStop();
    Grd5OpacityStop readOpacityStop();
    std::shared_ptr<Grd5Color> readColor();
    bool readRgbColorStandard(std::shared_ptr<Grd5RgbColor> rgbColor);
    bool readRgbColorFloat(std::shared_ptr<Grd5RgbColor> rgbColor);
    std::shared_ptr<Grd5HsvColor> readHsvColor();
    std::shared_ptr<Grd5LabColor> readLabColor();
    std::shared_ptr<Grd5CmykColor> readCmykColor();
    Grd5ColorModelType getColorModelType(std::string typeName);
    std::shared_ptr<Grd5NoiseGradient> readNoiseGradient();
    Grd5Enum readEnum(std::string expectedName);
    uint32_t readNamedLong(std::string expectedName);
    bool readNamedBool(std::string expectedName);
    Grd5Object readObject();
    Grd5Ucs2String readText(std::string expectedName);
    Grd5Extrema readExtrema(std::string expectedName);

    FILE* file;
    const std::map<std::string, Grd5Type> keywordMap = {
        {"bool", TYPE_BOOL},
        {"doub", TYPE_DOUBLE},
        {"enum", TYPE_ENUM},
        {"long", TYPE_LONG},
        {"Objc", TYPE_OBJECT},
        {"tdta", TYPE_TDTA},
        {"TEXT", TYPE_TEXT},
        {"patt", TYPE_PATTERN},
        {"UntF", TYPE_UNTF},
        {"VlLs", TYPE_VAR_LEN_LIST}
    };
    const std::map<std::string, Grd5GradientType> gradientTypeMap = {
        {"CstS", GRADIENT_SOLID},
        {"ClNs", GRADIENT_NOISE}
    };
    const std::map<std::string, Grd5ColorModelType> colorModelTypeMap = {
        {"RGBC", COLOR_MODEL_RGB},
        {"HSBC", COLOR_MODEL_HSV},
        {"HSBl", COLOR_MODEL_HSV},
        {"LbCl", COLOR_MODEL_LAB},
        {"CMYC", COLOR_MODEL_CMYK},
        {"Grsc", COLOR_MODEL_GRAYSCALE},
        {"FrgC", COLOR_MODEL_FOREGROUND},
        {"BckC", COLOR_MODEL_BACKGROUND},
        {"BkCl", COLOR_MODEL_BOOK},
        {"UnsC", COLOR_MODEL_UNSPECIFIED}
    };
};

}