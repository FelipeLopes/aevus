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
    Iterator(const CLQueuedContext& context, Flame flame, FlameCL flameCL,
        std::vector<IterationState>& stateVec,
        std::vector<uint8_t>& xformDistVec, std::vector<ColorCL>& paletteVec);
    void run();
    void readOutput(std::vector<float>& arr);
private:
    const CLQueuedContext& context;
    CLExecutable kernel;

    CLArg<FlameCL> flameCL;
    CLReadWriteBufferArg<IterationState> stateArg;
    CLReadOnlyBufferArg<XFormCL> xformArg;
    CLReadOnlyBufferArg<uint8_t> xformDistArg;
    CLReadOnlyBufferArg<ColorCL> paletteArg;
    CLWriteOnlyBufferArg<float> outputArg;
};

}