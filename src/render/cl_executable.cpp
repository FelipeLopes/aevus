#include "cl_executable.hpp"
#include <fstream>
#include <sstream>

namespace render {

CLExecutable::CLExecutable(std::string filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer<<file.rdbuf();
    auto source = buffer.str();
    printf("%s",source.c_str());
}

}