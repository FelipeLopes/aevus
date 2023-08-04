#pragma once

#include "../clwrap/cl_arg.hpp"
#include "../clwrap/cl_buffer.hpp"
#include "../clwrap/cl_context.hpp"
#include "../clwrap/cl_executable.hpp"
#include "../core/flame.hpp"

namespace render {

class Iterator {
public:
    Iterator(const clwrap::CLQueuedContext& context, std::shared_ptr<core::Flame> flame);
    void writeImage(std::string filename, std::vector<float>& arr);
private:
    const clwrap::CLQueuedContext& context;
    clwrap::CLExecutable kernel;
    const int width, height, scale, quality;
    const double brightness;
    const core::ColorCL background;

    clwrap::CLArg<core::FlameCL> flameCL;
    clwrap::CLReadWriteBufferArg<core::IterationState> stateArg;
    clwrap::CLReadOnlyBufferArg<core::XFormCL> xformArg;
    clwrap::CLReadOnlyBufferArg<uint8_t> xformDistArg;
    clwrap::CLReadOnlyBufferArg<core::ColorCL> paletteArg;
    clwrap::CLReadWriteBufferArg<float> histogramArg;

    static const int GLOBAL_WORK_SIZE = 1024;
    static const int LOCAL_WORK_SIZE = 64;
};

}