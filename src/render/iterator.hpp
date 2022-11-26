#pragma once

#include "cl_buffer.hpp"
#include "cl_context.hpp"
#include "flame.hpp"
#include "iteration_state.hpp"

namespace render {

class Iterator {
public:
    Iterator(Flame flame, const CLContext& context, const CLQueue& queue);
private:
    const CLContext& context;
    const CLQueue& queue;

    FlameCL flameCL;
    ReadWriteCLBuffer<IterationState> stateBuf;
    ReadOnlyCLBuffer<XFormCL> xformBuf;
    ReadOnlyCLBuffer<uint8_t> xformDistBuf;
    ReadOnlyCLBuffer<ColorCL> paletteBuf;
    WriteOnlyCLBuffer<float> outputBuf;
};

}