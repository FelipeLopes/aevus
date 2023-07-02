#pragma once

#include "cl_arg.hpp"
#include <vector>

namespace render {

class ToneMapper {
public:
    ToneMapper(const CLQueuedContext& context, float scale, float slope,
        std::vector<float>& hist);
private:
    const CLQueuedContext& context;
    CLExecutable kernel;

    CLArg<float> scaleArg;
    CLArg<float> slopeArg;
    CLReadWriteBufferArg<float> histArg;
};

}