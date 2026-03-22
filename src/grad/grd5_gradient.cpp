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
    printf("%d\n",readGradientListLength());
    return Grd5GradientList();
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

void Grd5Stream::readBytes(uint32_t len, char* arr) {
    if (fread(arr, 1, len, file) != len) {
        raiseFileStreamError();
    }
}

void Grd5Stream::parseNamedType(std::string expectedName, Grd5Type expectedType) {
    auto typeName = readGrd5TypeNameString();
    if (typeName.content != expectedName) {
        raiseTypeNameMismatch();
    }
    auto grd5Type = readType();
    if (grd5Type == TYPE_UNKNOWN) {
        throw std::invalid_argument("Read unknown type");
    } else if (grd5Type != expectedType) {
        throw std::invalid_argument("Type keyword mismatch");
    }
}

Grd5TypeNameString Grd5Stream::readGrd5TypeNameString() {
    uint32_t len = readUint32();
    Grd5TypeNameString ans;
    if (len == 0) {
        len = 4;
    }
    char* content;
    if ((content = (char*)malloc(len+1)) == NULL) {
        auto ec = std::error_code(errno, std::generic_category());
        throw std::system_error(ec, "Could not allocate memory");
    } else {
        readBytes(len, content);
        content[len] = '\0';
        ans.content = content;
        free(content);
    }
    return ans;
}

uint32_t Grd5Stream::readVllLength(std::string expectedName) {
    parseNamedType(expectedName, TYPE_VAR_LEN_LIST);
    return readUint32();
}

uint32_t Grd5Stream::readGradientListLength() {
    return readVllLength("GrdL");
}

}