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
    std::shared_ptr<std::vector<core::IterationState>> stateVec;
    std::shared_ptr<std::vector<core::XFormCL>> xformVec;
    std::shared_ptr<std::vector<uint8_t>> xformDistVec;
    std::shared_ptr<std::vector<core::ColorCL>> paletteVec;
    int iters;
};

class Iterator {
public:
    Iterator(clwrap::CLQueuedContext& context);
    IteratorParams extractParams(core::Flame* flame);
    void runAsync(IteratorParams params,
        std::function<void(std::shared_ptr<std::vector<float>>)> block);
private:
    void writePAMImage(std::stringstream& out, std::vector<float>& arr);
    clwrap::CLQueuedContext& context;
    Kernel kernel;

    clwrap::CLArg<core::FlameCL> flameArg;
    clwrap::CLReadWriteBufferArg<core::IterationState> stateArg;
    clwrap::CLReadOnlyBufferArg<core::XFormCL> xformArg;
    clwrap::CLReadOnlyBufferArg<uint8_t> xformDistArg;
    clwrap::CLReadOnlyBufferArg<core::ColorCL> paletteArg;
    clwrap::CLReadWriteBufferArg<float> histogramArg;
    clwrap::CLArg<int> itersArg;

    static const int GLOBAL_WORK_SIZE = 4096;
    static const int LOCAL_WORK_SIZE = 256;
};

}