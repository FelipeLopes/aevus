#pragma once

#include "../clwrap/cl_arg.hpp"
#include "../core/flame.hpp"
#include <sstream>
#include <vector>

namespace render {

class ToneMapper {
public:
    ToneMapper(const clwrap::CLQueuedContext& context);
    void setup(core::Flame* flame, std::vector<float>& hist);
    void run();
    void writePNMImage(std::stringstream& out);
private:
    const clwrap::CLQueuedContext& context;
    clwrap::CLExecutable kernel;

    clwrap::CLArg<float> aArg;
    clwrap::CLArg<float> bArg;
    clwrap::CLReadWriteBufferArg<float> histArg;

    int width, height;
    core::ColorCL background;

    static const int LOCAL_WORK_SIZE = 64;
};

}