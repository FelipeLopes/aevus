#pragma once

#include "cl_arg.hpp"
#include <vector>

namespace render {

class ToneMapper {
public:
    ToneMapper(const CLQueuedContext& context, int area, float a, float b,
        std::vector<float>& hist);
private:
    const CLQueuedContext& context;
    const int area;
    CLExecutable kernel;

    CLArg<float> aArg;
    CLArg<float> bArg;
    CLReadWriteBufferArg<float> histArg;

    static const int LOCAL_WORK_SIZE = 64;
};

}