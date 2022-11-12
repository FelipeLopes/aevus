#pragma once

#include "cl_context.hpp"

namespace render {

class OpenCL {
public:
    static OpenCL& getInstance() {
        static OpenCL instance;
        return instance;
    }
    CLContext getContext();
private:
    OpenCL() { }
    OpenCL(OpenCL const&) = delete;
    void operator= (OpenCL const&) = delete;
};

}