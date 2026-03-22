#pragma once

#include <cstdio>
#include <inttypes.h>
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

class Grd5GradientStop { };

class Grd5OpacityStop: public Grd5GradientStop {

};

class Grd5ColorStop: public Grd5GradientStop {

};

class Grd5Gradient {
public:
    std::string title;
};

class Grd5SolidGradient: public Grd5Gradient {
public:
    double smoothness;
    std::vector<Grd5OpacityStop> opacityStops;
    std::vector<Grd5ColorStop> colorStops;
};

class Grd5NoiseGradient: public Grd5Gradient {
public:
    bool showTransparency, vectorColor;
    uint32_t seed, smoothness;
    int model;
    Grd5Extrema min, max;
};

class Grd5GradientList {
public:
    std::vector<Grd5Gradient> gradients;
};

class Grd5String {
public:
    std::string content;
};

class Grd5TypeNameString: public Grd5String {

};

class Grd5Ucs2String: public Grd5String {

};

class Grd5TdtaString: public Grd5String {

};

class Grd5Stream {
public:
    Grd5Stream(const char* filename);
    Grd5GradientList readGradientList();
private:
    void raiseFileStreamError();
    void raiseTypeNameMismatch();
    void readBytes(uint32_t len, char* arr);
    uint16_t readUint16();
    uint32_t readUint32();
    Grd5Type readType();
    Grd5TypeNameString readGrd5TypeNameString();
    void parseNamedType(std::string expectedName, Grd5Type expectedType);
    uint32_t readVllLength(std::string expectedName);
    uint32_t readGradientListLength();

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
};

}