#pragma once

#include "cl_arg.hpp"
#include "cl_buffer.hpp"
#include "cl_context.hpp"
#include "cl_executable.hpp"
#include "flame.hpp"
#include "iteration_state.hpp"

namespace render {

class Iterator {
public:
    Iterator(Flame flame, const CLQueuedContext& context);
    void run();
    void readOutput(std::vector<float>& arr);
private:
    const CLQueuedContext& context;
    const CLQueue& queue;

    CLExecutable kernel;

    CLArg<FlameCL> flameCL;
    ReadWriteCLBuffer<IterationState> stateBuf;
    ReadOnlyCLBuffer<XFormCL> xformBuf;
    ReadOnlyCLBuffer<uint8_t> xformDistBuf;
    ReadOnlyCLBuffer<ColorCL> paletteBuf;
    WriteOnlyCLBuffer<float> outputBuf;
};

}