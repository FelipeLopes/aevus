#pragma once

#include "../clwrap/cl_arg.hpp"
#include <vector>

namespace render {

class ToneMapper {
public:
    ToneMapper(const clwrap::CLQueuedContext& context, int area, float a, float b,
        std::vector<float>& hist);
    void readOutput(std::vector<float>& arr);
private:
    const clwrap::CLQueuedContext& context;
    const int area;
    clwrap::CLExecutable kernel;

    clwrap::CLArg<float> aArg;
    clwrap::CLArg<float> bArg;
    clwrap::CLReadWriteBufferArg<float> histArg;

    static const int LOCAL_WORK_SIZE = 64;
};

}