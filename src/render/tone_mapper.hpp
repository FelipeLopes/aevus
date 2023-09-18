#pragma once

#include "../clwrap/cl_arg.hpp"
#include "../core/flame.hpp"
#include "kernel.hpp"
#include <memory>
#include <sstream>
#include <vector>

namespace render {

class ToneMapper {
public:
    ToneMapper(clwrap::CLQueuedContext& context);
    void setup(core::Flame* flame, std::shared_ptr<std::vector<float>> hist);
    void runAsync(std::function<void(std::shared_ptr<std::vector<float>>)> block);
private:
    clwrap::CLQueuedContext& context;
    Kernel kernel;

    clwrap::CLArg<float> aArg;
    clwrap::CLArg<float> bArg;
    clwrap::CLReadWriteBufferArg<float> histArg;

    int width, height;
    core::ColorCL background;

    static const int LOCAL_WORK_SIZE = 64;
};

}