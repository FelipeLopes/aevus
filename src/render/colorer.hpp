#pragma once

#include "../clwrap/cl_arg.hpp"
#include "../clwrap/cl_context.hpp"
#include "../core/flame.hpp"
#include "kernel.hpp"
#include <cstdint>

namespace render {

struct ColorerParams {
    int width, height;
    float gamma, vibrancy;
    core::ColorCL background;
    core::ClippingMode clippingMode;
};

class Colorer {
public:
    Colorer(clwrap::CLQueuedContext& context);
    void extractParams(core::Flame* flame, ColorerParams& params);
    std::shared_ptr<clwrap::CLEvent> runAsync(ColorerParams& params, std::vector<float>& density);
    void read(std::shared_ptr<clwrap::CLEvent> event, std::vector<uint8_t>& imageData);
private:
    clwrap::CLQueuedContext& context;
    Kernel kernel;

    clwrap::CLReadOnlyBufferArg<float> densityArg;
    clwrap::CLArg<core::ColorCL> backgroundArg;
    clwrap::CLArg<float> invGammaArg;
    clwrap::CLArg<float> vibrancyArg;
    clwrap::CLArg<core::ClippingMode> modeArg;
    clwrap::CLArg<int> chunkArg;
    clwrap::CLWriteOnlyBufferArg<uint8_t> imageArg;

    std::vector<uint8_t> imageVec;

    static const int GLOBAL_WORK_SIZE = 4096;
    static const int LOCAL_WORK_SIZE = 256;
};

}