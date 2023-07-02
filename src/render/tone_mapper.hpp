#pragma once

#include "cl_arg.hpp"
#include <vector>

namespace render {

class ToneMapper {
public:
    ToneMapper(const CLQueuedContext& context, float a, float b,
        std::vector<float>& hist);
private:
    const CLQueuedContext& context;
    CLExecutable kernel;

    CLArg<float> aArg;
    CLArg<float> bArg;
    CLReadWriteBufferArg<float> histArg;
};

}