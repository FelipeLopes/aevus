#pragma once

#include "../clwrap/cl_arg.hpp"
#include "../core/flame.hpp"
#include "kernel.hpp"
#include <memory>
#include <sstream>
#include <vector>

namespace render {

struct ToneMapperParams {
    int width, height;
    double a, b;
};

class ToneMapper {
public:
    ToneMapper(clwrap::CLQueuedContext& context);
    void extractParams(core::Flame* flame, ToneMapperParams& params);
    std::shared_ptr<clwrap::CLEvent> runAsync(ToneMapperParams& params, std::vector<float>& hist);
    void read(std::shared_ptr<clwrap::CLEvent> event, std::vector<float>& imageData);
private:
    clwrap::CLQueuedContext& context;
    Kernel kernel;

    clwrap::CLArg<float> aArg;
    clwrap::CLArg<float> bArg;
    clwrap::CLReadWriteBufferArg<float> histArg;
    clwrap::CLArg<int> chunkArg;

    static const int GLOBAL_WORK_SIZE = 4096;
    static const int LOCAL_WORK_SIZE = 256;
};

}