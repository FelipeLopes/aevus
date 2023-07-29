#pragma once

#include "cl_arg.hpp"
#include "cl_buffer.hpp"
#include "cl_context.hpp"
#include "cl_executable.hpp"
#include "color_cl.hpp"
#include "flame.hpp"
#include "iteration_state.hpp"
#include "../core/flame.hpp"

namespace render {

class Iterator {
public:
    Iterator(const CLQueuedContext& context, core::Flame coreFlame, Flame flame,
        int quality, double brightness, int initialIters);
    void writeImage(std::string filename, std::vector<float>& arr);
private:
    const CLQueuedContext& context;
    CLExecutable kernel;
    const int width, height, scale, quality;

    CLArg<FlameCL> flameCL;
    CLReadWriteBufferArg<IterationState> stateArg;
    CLReadOnlyBufferArg<XFormCL> xformArg;
    CLReadOnlyBufferArg<uint8_t> xformDistArg;
    CLReadOnlyBufferArg<ColorCL> paletteArg;
    CLReadWriteBufferArg<float> histogramArg;

    static const int GLOBAL_WORK_SIZE = 1024;
    static const int LOCAL_WORK_SIZE = 64;
};

}