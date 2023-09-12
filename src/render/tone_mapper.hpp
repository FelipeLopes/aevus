#pragma once

#include "../clwrap/cl_arg.hpp"
#include "../core/flame.hpp"
#include "kernel.hpp"
#include <sstream>
#include <vector>

namespace render {

class Renderer;

class ToneMapper {
public:
    ToneMapper(const clwrap::CLQueuedContext& context);
    void setup(core::Flame* flame, std::vector<float>& hist);
    void run();
    void runAsync(Renderer* renderer, void (*block)(Renderer* renderer));
    void writePNMImage(std::stringstream& out);
private:
    const clwrap::CLQueuedContext& context;
    Kernel kernel;

    clwrap::CLArg<float> aArg;
    clwrap::CLArg<float> bArg;
    clwrap::CLReadWriteBufferArg<float> histArg;

    int width, height;
    core::ColorCL background;

    static const int LOCAL_WORK_SIZE = 64;
};

}