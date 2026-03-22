#include "grd5_gradient.hpp"
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <system_error>

namespace grad {

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

Grd5GradientList Grd5Stream::readGradientList() {
    auto ans = Grd5GradientList();
    int n = readVllLength("GrdL");
    for (int i=0; i<std::min(1,n); i++) {
        ans.gradients.push_back(readGradient());
    }
    return ans;
}

Grd5Gradient Grd5Stream::readGradient() {
    Grd5Gradient ans;
    if (readType() != TYPE_OBJECT) {
        throw std::invalid_argument("Gradient outer object not found");
    }
    readObject();
    parseNamedType("Grad", TYPE_OBJECT);
    int ncomp = readObject().value;
    ans.title = readGradientTitle();
    auto gradType = readGradientType();
    if (gradType == GRADIENT_SOLID) {
        if (ncomp != 5) {
            throw std::invalid_argument("Solid gradient has invalid number of components");
        } else {
            return readSolidGradient();
        }
    } else if (gradType == GRADIENT_NOISE) {
        return readNoiseGradient();
    } else {
        throw std::invalid_argument("Unknown gradient type");
    }
}

Grd5SolidGradient Grd5Stream::readSolidGradient() {
    Grd5SolidGradient ans;
    ans.smoothness = readNamedDouble("Intr");
    ans.colorStops.resize(readVllLength("Clrs"));
    for (int i=0; i<1; i++) {
        ans.colorStops[i] = readColorStop();
    }
    return ans;
}

Grd5ColorStop Grd5Stream::readColorStop() {
    auto type = readType();
    if (type != TYPE_OBJECT) {
        raiseTypeIdMismatch();
    }
    int ncomp = readObject().value;
    bool hasUserColor;
    switch (ncomp) {
        case 3: hasUserColor = false; break;
        case 4: hasUserColor = true; break;
        default:
            throw std::invalid_argument("Invalid number of components in color stop");
    }
    if (hasUserColor) {
        printf("color stop with user color\n");
    }
    return Grd5ColorStop();
}

Grd5NoiseGradient Grd5Stream::readNoiseGradient() {
    return Grd5NoiseGradient();
}

Grd5Ucs2String Grd5Stream::readGradientTitle() {
    return readText("Nm  ");
}

Grd5Stream::Grd5GradientType Grd5Stream::readGradientType() {
    auto enumVar = readEnum("GrdF");
    if (enumVar.name.content != "GrdF") {
        raiseTypeNameMismatch();
    }
    auto it = gradientTypeMap.find(enumVar.subname.content);
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
    return *(double*)(&val);
}

Grd5Stream::Grd5Type Grd5Stream::readType() {
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

void Grd5Stream::readBytes(uint32_t len, char* arr) {
    if (fread(arr, 1, len, file) != len) {
        raiseFileStreamError();
    }
}

void Grd5Stream::parseNamedType(std::string expectedName, Grd5Type expectedType) {
    auto typeName = readTypeNameString();
    if (typeName.content != expectedName) {
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
    if ((content = (char*)malloc(len+1)) == NULL) {
        auto ec = std::error_code(errno, std::generic_category());
        throw std::system_error(ec, "Could not allocate memory");
    } else {
        readBytes(len, content);
        content[len] = '\0';
        str.content = content;
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

}