#include "colorer.hpp"

using clwrap::CLQueuedContext;
using std::shared_ptr;
using std::vector;

namespace render {

Colorer::Colorer(CLQueuedContext& context_):
    context(context_),
    kernel(context, "coloring"),
    densityArg(&kernel, 0),
    backgroundArg(&kernel, 1, core::ColorCL()),
    invGammaArg(&kernel, 2, 1.0f),
    vibrancyArg(&kernel, 3, 1.0f),
    modeArg(&kernel, 4, core::WHITE),
    chunkArg(&kernel, 5, 0),
    imageArg(&kernel, 6) { }

void Colorer::extractParams(core::Flame* flame, ColorerParams& params) {
    auto sz = flame->size.value();
    params.width = sz.width;
    params.height = sz.height;
    params.gamma = flame->gamma.value();
    params.vibrancy = flame->vibrancy.value();
    params.background = flame->background.value().toColorCL();
    params.clippingMode = flame->clipping.value().mode;
}

shared_ptr<clwrap::CLEvent> Colorer::runAsync(ColorerParams& params, vector<float>& density) {
    densityArg.lazy(density);
    backgroundArg.set(params.background);
    invGammaArg.set(1.0/params.gamma);
    vibrancyArg.set(params.vibrancy);
    modeArg.set(params.clippingMode);
    chunkArg.set(ceil(1.0*params.width*params.height/GLOBAL_WORK_SIZE));
    int imageVecSize = 3*ceil(1.0*params.width*params.height/GLOBAL_WORK_SIZE) *
        GLOBAL_WORK_SIZE;
    imageArg.resize(imageVecSize);
    return kernel.runAsync(GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);
}

void Colorer::read(shared_ptr<clwrap::CLEvent> event, vector<uint8_t>& imageData) {
    imageArg.getAfterEvent(event, imageData);
}

}