#include "grd5_gradient.hpp"
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

uint16_t Grd5Stream::readUint16() {
    uint16_t val;
    if (fread(&val, 2, 1, file) != 1) {
        raiseFileStreamError();
    }
    return be16toh(val);
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

Grd5GradientList Grd5Stream::readGradientList() {
    return Grd5GradientList();
}

}