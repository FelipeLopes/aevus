#pragma once

#include "../clwrap/cl_arg.hpp"
#include "../clwrap/cl_buffer.hpp"
#include "../clwrap/cl_context.hpp"
#include "../clwrap/cl_executable.hpp"
#include "../core/flame.hpp"
#include "kernel.hpp"
#include <sstream>

namespace render {

class Renderer;

class Iterator {
public:
    Iterator(const clwrap::CLQueuedContext& context, std::stringstream& out);
    void setup(core::Flame* flame);
    void run();
    void runAsync(Renderer* renderer, void (*block)(Renderer* renderer));
    void getRenderData();
    void writePNMImage();
    std::vector<float> renderData;
private:
    void writePAMImage(std::stringstream& out, std::vector<float>& arr);
    const clwrap::CLQueuedContext& context;
    std::stringstream& out;
    Kernel kernel;
    int width, height, scale, quality;
    double brightness;
    core::ColorCL background;

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