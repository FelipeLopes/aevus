#pragma once

#include "../clwrap/cl_arg.hpp"
#include "../clwrap/cl_buffer.hpp"
#include "../clwrap/cl_context.hpp"
#include "../clwrap/cl_executable.hpp"
#include "../core/flame.hpp"
#include <sstream>

namespace render {

class Iterator {
public:
    Iterator(const clwrap::CLQueuedContext& context, std::stringstream& out);
    void setFlame(core::Flame* flame);
    void render();
    void writePNMImage();
private:
    void writePAMImage(std::stringstream& out, std::vector<float>& arr);
    const clwrap::CLQueuedContext& context;
    std::stringstream& out;
    clwrap::CLExecutable kernel;
    int width, height, scale, quality;
    double brightness;
    core::ColorCL background;

    clwrap::CLArg<core::FlameCL> flameArg;
    clwrap::CLReadWriteBufferArg<core::IterationState> stateArg;
    clwrap::CLReadOnlyBufferArg<core::XFormCL> xformArg;
    clwrap::CLReadOnlyBufferArg<uint8_t> xformDistArg;
    clwrap::CLReadOnlyBufferArg<core::ColorCL> paletteArg;
    clwrap::CLReadWriteBufferArg<float> histogramArg;

    std::vector<float> renderData;

    static const int GLOBAL_WORK_SIZE = 1024;
    static const int LOCAL_WORK_SIZE = 64;
};

}