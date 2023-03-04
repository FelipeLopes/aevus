#pragma once

#include "cl_arg.hpp"
#include "cl_buffer.hpp"
#include "cl_context.hpp"
#include "cl_executable.hpp"
#include "color_cl.hpp"
#include "flame.hpp"
#include "iteration_state.hpp"

namespace render {

class Iterator {
public:
    Iterator(const CLQueuedContext& context, Flame flame, int globalWorkSize, int localWorkSize,
        int initialIters, int histIters);
    void readHistogram(std::vector<float>& arr);
private:
    const CLQueuedContext& context;
    CLExecutable kernel;
    const int globalWorkSize, localWorkSize;

    CLArg<FlameCL> flameCL;
    CLReadWriteBufferArg<IterationState> stateArg;
    CLReadOnlyBufferArg<XFormCL> xformArg;
    CLReadOnlyBufferArg<uint8_t> xformDistArg;
    CLReadOnlyBufferArg<ColorCL> paletteArg;
    CLReadWriteBufferArg<float> histogramArg;
};

}