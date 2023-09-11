#pragma once

#include <map>
#include "../clwrap/cl_executable.hpp"

extern char _binary_cl_iterate_cl_start[];
extern char _binary_cl_iterate_cl_end[];
extern char _binary_cl_mapping_cl_start[];
extern char _binary_cl_mapping_cl_end[];

namespace render {

class Kernel : public clwrap::CLExecutable {
public:
    Kernel(const clwrap::CLQueuedContext& context, std::string name);
private:
    static const std::map<std::string, std::pair<char*, char*>> lookup;
};

}