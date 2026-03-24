#include "grd5_gradient.hpp"
#include "string_encoding.hpp"
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <system_error>

namespace grad {

std::string Grd5TypeNameString::toString() {
    return std::string(content.begin(), content.end());
}

std::string Grd5TdtaString::toString() {
    return std::string(content.begin(), content.end());
}

std::string Grd5Ucs2String::toString() {
    return Utf8Ucs2Converter::ucs2ToUtf8(std::wstring(content.begin(), content.end()));
}

Grd5RgbColor::Grd5RgbColor(): Grd5RgbColor(0, 0, 0) { }

Grd5RgbColor::Grd5RgbColor(double r, double g, double b): Rd(r), Grn(g), Bl(b) { }

Grd5HsvColor::Grd5HsvColor() { }

Grd5LabColor::Grd5LabColor() { }

Grd5CmykColor::Grd5CmykColor() { }

Grd5GrayScaleColor::Grd5GrayScaleColor(double val_): val(val_) { }

Grd5BookColor::Grd5BookColor(Grd5Ucs2String Bk_, Grd5Ucs2String Nm_, uint32_t bookId_, Grd5TdtaString bookKey_):
    Bk(Bk_), Nm(Nm_), bookId(bookId_), bookKey(bookKey_) { }

Grd5Stream::Grd5Stream(const char* filename) {
    file = fopen(filename, "rb");
    if (file == NULL) {
        auto ec = std::error_code(errno, std::generic_category());
        throw std::system_error(ec, "Could not open file");
    }
    char buf[5];
    if (fread(buf, 1, 4, file) != 4) {
        raiseFileStreamError();
    }
    if (strncmp(buf, "8BGR", 4) != 0) {
        throw std::invalid_argument("Failed read of magic number");
    }
    uint16_t version = readUint16();
    if (version != 5) {
        throw std::invalid_argument("Bad version in GRD5 header");
    }
    // Skip unknown/padding bytes
    if (fseek(file, 22, SEEK_CUR) != 0) {
        throw std::invalid_argument("Failed seek of padding bytes");
    }
}

Grd5Stream::~Grd5Stream() {
    fclose(file);
}

Grd5GradientList Grd5Stream::readGradientList() {
    auto ans = Grd5GradientList();
    int n = readVllLength("GrdL");
    ans.gradients.reserve(n);
    for (int i=0; i<n; i++) {
        auto grad = readGradient();
        ans.gradients.push_back(grad);
    }
    return ans;
}

std::shared_ptr<Grd5Gradient> Grd5Stream::readGradient() {
    if (readType() != TYPE_OBJECT) {
        throw std::invalid_argument("Gradient outer object not found");
    }
    readObject();
    parseNamedType("Grad", TYPE_OBJECT);
    int ncomp = readObject().value;
    auto title = readText("Nm  ");
    auto gradType = readGradientType();
    if (gradType == GRADIENT_SOLID) {
        if (ncomp != 5) {
            raiseComponentMismatch();
        } else {
            auto solid = readSolidGradient();
            solid->title = title;
            return solid;
        }
    } else if (gradType == GRADIENT_NOISE) {
        if (ncomp != 9) {
            raiseComponentMismatch();
        } else {
            auto noise = readNoiseGradient();
            noise->title = title;
            return noise;
        }
    } else {
        throw std::invalid_argument("Unknown gradient type");
    }
    return NULL;
}

std::shared_ptr<Grd5SolidGradient> Grd5Stream::readSolidGradient() {
    auto ans = std::make_shared<Grd5SolidGradient>();
    ans->smoothness = readNamedDouble("Intr");
    ans->colorStops.resize(readVllLength("Clrs"));
    for (int i=0; i<ans->colorStops.size(); i++) {
        ans->colorStops[i] = readColorStop();
    }
    ans->opacityStops.resize(readVllLength("Trns"));
    for (int i=0; i<ans->opacityStops.size(); i++) {
        ans->opacityStops[i] = readOpacityStop();
    }
    return ans;
}

Grd5ColorStop Grd5Stream::readColorStop() {
    Grd5ColorStop ans;
    auto type = readType();
    if (type != TYPE_OBJECT) {
        raiseTypeIdMismatch();
    }
    int ncomp = readObject().value;
    bool hasUserColor = false;
    switch (ncomp) {
        case 3: break;
        case 4: hasUserColor = true; break;
        default: raiseComponentMismatch();
    }
    if (hasUserColor) {
        ans.color = readColor();
    }
    auto enumVal = readEnum("Type");
    if (enumVal.name.toString() != "Clry") {
        raiseTypeNameMismatch();
    }
    if (hasUserColor && enumVal.subname.toString() != "UsrS") {
        raiseTypeNameMismatch();
    }
    if (!hasUserColor) {
        if (enumVal.subname.toString() == "BckC") {
            ans.color = std::make_shared<Grd5BackgroundColor>();
        } else if (enumVal.subname.toString() == "FrgC") {
            ans.color = std::make_shared<Grd5ForegroundColor>();
        } else {
            raiseTypeNameMismatch();
        }
    }
    ans.Lctn = readNamedLong("Lctn");
    ans.Mdpn = readNamedLong("Mdpn");
    return ans;
}

std::shared_ptr<Grd5Color> Grd5Stream::readColor() {
    parseNamedType("Clr ", TYPE_OBJECT);
    auto obj = readObject();
    int ncomp = obj.value;
    auto type = getColorModelType(obj.typeName.toString());
    if (type == COLOR_MODEL_UNKNOWN) {
        throw std::invalid_argument("Unknown color model type");
    }
    switch (ncomp) {
        case 0: return std::make_shared<Grd5UnspecifiedColor>();
        case 1: {
            if (type != COLOR_MODEL_GRAYSCALE) {
                raiseComponentMismatch();
            }
            return std::make_shared<Grd5GrayScaleColor>(readNamedDouble("Gry "));
        }
        case 2: {
            if (type != COLOR_MODEL_BOOK) {
                raiseComponentMismatch();
            }
            auto Bk = readText("Bk  ");
            auto Nm = readText("Nm  ");
            return std::make_shared<Grd5BookColor>(Bk, Nm, 0, Grd5TdtaString());
        }
        case 3: {
            switch(type) {
                case COLOR_MODEL_RGB: {
                    auto ans = std::make_shared<Grd5RgbColor>();
                    if (!readRgbColorStandard(ans) && !readRgbColorFloat(ans)) {
                        throw std::invalid_argument("Could not read RGB color");
                    }
                    return ans;
                }
                case COLOR_MODEL_HSV: {
                    return readHsvColor();
                }
                case COLOR_MODEL_LAB: {
                    return readLabColor();
                }
                default:
                    raiseComponentMismatch();
            }
        }
        case 4: {
            switch(type) {
                case COLOR_MODEL_CMYK: {
                    return readCmykColor();
                }
                case COLOR_MODEL_BOOK: {
                    auto Bk = readText("Bk  ");
                    auto Nm = readText("Nm  ");
                    auto bookID = readNamedLong("bookID");
                    parseNamedType("bookKey", TYPE_TDTA);
                    auto bookKey = readTdtaString();
                    return std::make_shared<Grd5BookColor>(Bk, Nm, bookID, bookKey);
                }
                default:
                    raiseComponentMismatch();
            }
        }
        default:
            raiseComponentMismatch();
    }
    return NULL;
}

bool Grd5Stream::readRgbColorStandard(std::shared_ptr<Grd5RgbColor> rgbColor) {
    fpos_t pos;
    if (fgetpos(file, &pos) != 0) {
        auto ec = std::error_code(errno, std::generic_category());
        throw std::system_error(ec, "Failed to get stream position");
    }
    try {
        rgbColor->Rd  = readNamedDouble("Rd  ");
        rgbColor->Grn = readNamedDouble("Grn ");
        rgbColor->Bl  = readNamedDouble("Bl  ");
        return true;
    } catch (std::invalid_argument e) {
        if (fsetpos(file, &pos) != 0) {
            auto ec = std::error_code(errno, std::generic_category());
            throw std::system_error(ec, "Failed to set stream position");
        }
        return false;
    }
}

bool Grd5Stream::readRgbColorFloat(std::shared_ptr<Grd5RgbColor> rgbColor) {
    fpos_t pos;
    if (fgetpos(file, &pos) != 0) {
        auto ec = std::error_code(errno, std::generic_category());
        throw std::system_error(ec, "Failed to get stream position");
    }
    try {
        rgbColor->Rd  = readNamedDouble("redFloat");
        rgbColor->Grn = readNamedDouble("greenFloat");
        rgbColor->Bl  = readNamedDouble("blueFloat");
        return true;
    } catch (std::invalid_argument e) {
        if (fsetpos(file, &pos) != 0) {
            auto ec = std::error_code(errno, std::generic_category());
            throw std::system_error(ec, "Failed to set stream position");
        }
        return false;
    }
}

std::shared_ptr<Grd5HsvColor> Grd5Stream::readHsvColor() {
    auto ans = std::make_shared<Grd5HsvColor>();
    ans->H    = readUnitDouble("H   ", "#Ang");
    ans->Strt = readNamedDouble("Strt");
    ans->Brgh = readNamedDouble("Brgh");
    return ans;
}

std::shared_ptr<Grd5LabColor> Grd5Stream::readLabColor() {
    auto ans = std::make_shared<Grd5LabColor>();
    ans->Lmnc = readNamedDouble("Lmnc");
    ans->A    = readNamedDouble("A   ");
    ans->B    = readNamedDouble("B   ");
    return ans;
}

std::shared_ptr<Grd5CmykColor> Grd5Stream::readCmykColor() {
    auto ans = std::make_shared<Grd5CmykColor>();
    ans->Cyn  = readNamedDouble("Cyn ");
    ans->Mgnt = readNamedDouble("Mgnt");
    ans->Ylw  = readNamedDouble("Ylw ");
    ans->Blck = readNamedDouble("Blck");
    return ans;
}

Grd5ColorModelType Grd5Stream::getColorModelType(std::string typeName) {
    auto it = colorModelTypeMap.find(typeName);
    if (it == colorModelTypeMap.end()) {
        return COLOR_MODEL_UNKNOWN;
    } else {
        return it->second;
    }
}

Grd5OpacityStop Grd5Stream::readOpacityStop() {
    Grd5OpacityStop ans;
    auto type = readType();
    if (type != TYPE_OBJECT) {
        raiseTypeIdMismatch();
    }
    auto obj = readObject();
    int ncomp = obj.value;
    if (ncomp != 3) {
        raiseComponentMismatch();
    }
    if (obj.typeName.toString() != "TrnS") {
        raiseTypeNameMismatch();
    }
    ans.Opct = readUnitDouble("Opct", "#Prc");
    ans.Lctn = readNamedLong("Lctn");
    ans.Mdpn = readNamedLong("Mdpn");
    return ans;
}

std::shared_ptr<Grd5NoiseGradient> Grd5Stream::readNoiseGradient() {
    auto ans = std::make_shared<Grd5NoiseGradient>();
    ans->showTransparency = readNamedBool("ShTr");
    ans->vectorColor = readNamedBool("VctC");
    auto enumVar = readEnum("ClrS");
    if (enumVar.name.toString() != "ClrS") {
        raiseTypeNameMismatch();
    }
    ans->model = getColorModelType(enumVar.subname.toString());
    if (ans->model == COLOR_MODEL_UNKNOWN) {
        throw std::invalid_argument("Unknown color model type");
    }
    ans->seed = readNamedLong("RndS");
    ans->smoothness = readNamedLong("Smth");
    ans->min = readExtrema("Mnm ");
    ans->max = readExtrema("Mxm ");
    return ans;
}

Grd5Stream::Grd5GradientType Grd5Stream::readGradientType() {
    auto enumVar = readEnum("GrdF");
    if (enumVar.name.toString() != "GrdF") {
        raiseTypeNameMismatch();
    }
    auto it = gradientTypeMap.find(enumVar.subname.toString());
    if (it == gradientTypeMap.end()) {
        return GRADIENT_UNKNOWN;
    } else {
        return it->second;
    }
}

Grd5Enum Grd5Stream::readEnum(std::string expectedName) {
    Grd5Enum ans;
    parseNamedType(expectedName, TYPE_ENUM);
    ans.name = readTypeNameString();
    ans.subname = readTypeNameString();
    return ans;
}

uint32_t Grd5Stream::readNamedLong(std::string expectedName) {
    parseNamedType(expectedName, TYPE_LONG);
    return readUint32();
}

uint16_t Grd5Stream::readUint16() {
    uint16_t val;
    if (fread(&val, 2, 1, file) != 1) {
        raiseFileStreamError();
    }
    return be16toh(val);
}

uint32_t Grd5Stream::readUint32() {
    uint32_t val;
    if (fread(&val, 4, 1, file) != 1) {
        raiseFileStreamError();
    }
    return be32toh(val);
}

double Grd5Stream::readDouble() {
    uint64_t val;
    if (fread(&val, 8, 1, file) != 1) {
        raiseFileStreamError();
    }
    val = be64toh(val);
    return *(double*)(&val);
}

bool Grd5Stream::readBool() {
    unsigned char c;
    if (fread(&c, 1, 1, file) != 1) {
        raiseFileStreamError();
    }
    return (c != 0);
}

Grd5Type Grd5Stream::readType() {
    char buf[5];
    if (fread(&buf, 4, 1, file) != 1) {
        raiseFileStreamError();
    }
    buf[4] = '\0';
    std::string str = buf;
    auto it = keywordMap.find(str);
    if (it == keywordMap.end()) {
        return TYPE_UNKNOWN;
    } else {
        return it->second;
    }
}

void Grd5Stream::raiseFileStreamError() {
    if (feof(file)) {
        throw std::invalid_argument("EOF reached in file stream");
    }
    if (ferror(file)) {
        throw std::invalid_argument("Error reading stream");
    }
    throw std::invalid_argument("Unknown file stream error");
}

void Grd5Stream::raiseTypeNameMismatch() {
    throw std::invalid_argument("Typename mismatch");
}

void Grd5Stream::raiseTypeIdMismatch() {
    throw std::invalid_argument("Type ID mismatch");
}

void Grd5Stream::raiseComponentMismatch() {
    throw std::invalid_argument("Number of components mismatch");
}

void Grd5Stream::readBytes(uint32_t len, char* arr) {
    if (fread(arr, 1, len, file) != len) {
        raiseFileStreamError();
    }
}

void Grd5Stream::parseNamedType(std::string expectedName, Grd5Type expectedType) {
    auto typeName = readTypeNameString();
    if (typeName.toString() != expectedName) {
        raiseTypeNameMismatch();
    }
    auto grd5Type = readType();
    if (grd5Type == TYPE_UNKNOWN) {
        throw std::invalid_argument("Read unknown type");
    } else if (grd5Type != expectedType) {
        raiseTypeIdMismatch();
    }
}

double Grd5Stream::readNamedDouble(std::string expectedName) {
    parseNamedType(expectedName, TYPE_DOUBLE);
    return readDouble();
}

double Grd5Stream::readUnitDouble(std::string expectedName, std::string expectedUnit) {
    parseNamedType(expectedName, TYPE_UNTF);
    char unit[5];
    readBytes(4, unit);
    unit[4] = '\0';
    if (expectedUnit != unit) {
        throw std::invalid_argument("Unit mismatch in unit double");
    }
    return readDouble();
}

bool Grd5Stream::readNamedBool(std::string expectedName) {
    parseNamedType(expectedName, TYPE_BOOL);
    return readBool();
}

void Grd5Stream::readString(Grd5StringType type, uint32_t len, Grd5String& str) {
    switch (type) {
        case STRING_TDTA: break;
        case STRING_TYPENAME: {
            if (len == 0) {
                len = 4;
            }
            break;
        }
        case STRING_UCS2: len *= 2; break;
        default:
            throw std::invalid_argument("Bad string type");
    }
    char* content;
    if ((content = (char*)malloc(len)) == NULL) {
        auto ec = std::error_code(errno, std::generic_category());
        throw std::system_error(ec, "Could not allocate memory");
    } else {
        readBytes(len, content);
        str.content.reserve(len);
        for (int i=0; i<len; i++) {
            str.content.push_back(content[i]);
        }
        free(content);
    }
}

Grd5TypeNameString Grd5Stream::readTypeNameString() {
    uint32_t len = readUint32();
    Grd5TypeNameString ans;
    readString(STRING_TYPENAME, len, ans);
    return ans;
}

Grd5Ucs2String Grd5Stream::readUcs2String() {
    uint32_t len = readUint32();
    Grd5Ucs2String ans;
    readString(STRING_UCS2, len, ans);
    return ans;
}

Grd5TdtaString Grd5Stream::readTdtaString() {
    uint32_t len = readUint32();
    Grd5TdtaString ans;
    readString(STRING_TDTA, len, ans);
    return ans;
}

uint32_t Grd5Stream::readVllLength(std::string expectedName) {
    parseNamedType(expectedName, TYPE_VAR_LEN_LIST);
    return readUint32();
}

Grd5Object Grd5Stream::readObject() {
    Grd5Object obj;
    obj.displayName = readUcs2String();
    obj.typeName = readTypeNameString();
    obj.value = readUint32();
    return obj;
}

Grd5Ucs2String Grd5Stream::readText(std::string expectedName) {
    parseNamedType(expectedName, TYPE_TEXT);
    return readUcs2String();
}

Grd5Extrema Grd5Stream::readExtrema(std::string expectedName) {
    Grd5Extrema ans;
    uint32_t len = readVllLength(expectedName);
    ans.extremumList.resize(len);
    for (int i=0; i<len; i++) {
        ans.extremumList[i].type = readType();
        switch (ans.extremumList[i].type) {
            case TYPE_LONG:
                ans.extremumList[i].u = readUint32();
                break;
            case TYPE_DOUBLE:
                ans.extremumList[i].d = readDouble();
                break;
            default:
                raiseTypeIdMismatch();
        }
    }
    return ans;
}

}