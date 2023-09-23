#pragma once

#include "../clwrap/cl_arg.hpp"
#include "../clwrap/cl_buffer.hpp"
#include "../clwrap/cl_context.hpp"
#include "../clwrap/cl_executable.hpp"
#include "../core/flame.hpp"
#include "kernel.hpp"
#include <sstream>

namespace render {

struct IteratorParams {
    core::FlameCL flameCL;
    std::vector<core::IterationState> stateVec;
    std::vector<core::XFormCL> xformVec;
    std::vector<core::VariationCL> varVec;
    std::vector<float> paramVec;
    std::vector<uint8_t> xformDistVec;
    std::vector<core::ColorCL> paletteVec;
    float threshold;
    int iters;
};

class Iterator {
public:
    Iterator(clwrap::CLQueuedContext& context);
    void extractParams(core::Flame* flame, IteratorParams& params);
    void runAsync(IteratorParams& params,
        std::function<void(std::shared_ptr<std::vector<float>>)> block);
private:
    void writePAMImage(std::stringstream& out, std::vector<float>& arr);
    clwrap::CLQueuedContext& context;
    Kernel kernel;

    clwrap::CLArg<core::FlameCL> flameArg;
    clwrap::CLReadWriteBufferArg<core::IterationState> stateArg;
    clwrap::CLReadOnlyBufferArg<core::XFormCL> xformArg;
    clwrap::CLReadOnlyBufferArg<core::VariationCL> varArg;
    clwrap::CLReadOnlyBufferArg<float> paramArg;
    clwrap::CLReadOnlyBufferArg<uint8_t> xformDistArg;
    clwrap::CLReadOnlyBufferArg<core::ColorCL> paletteArg;
    clwrap::CLReadWriteBufferArg<float> histogramArg;
    clwrap::CLArg<float> thresholdArg;
    clwrap::CLArg<int> itersArg;

    static const int GLOBAL_WORK_SIZE = 4096;
    static const int LOCAL_WORK_SIZE = 256;
};

}