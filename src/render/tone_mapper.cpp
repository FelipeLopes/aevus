#include "tone_mapper.hpp"

using clwrap::CLQueuedContext;
using core::Flame;
using std::shared_ptr;
using std::stringstream;
using std::vector;

namespace render {

ToneMapper::ToneMapper(CLQueuedContext& context_):
    context(context_),
    kernel(context, "mapping"),
    aArg(&kernel, 0, 1.0),
    bArg(&kernel, 1, 1.0),
    histArg(&kernel, 2),
    chunkArg(&kernel, 3, 0) { }

void ToneMapper::extractParams(Flame* flame, ToneMapperParams& params) {
    auto sz = flame->size.value();
    params.width = sz.width;
    params.height = sz.height;
    int area = sz.width * sz.height;
    double scale = flame->scale.value();
    double quality = flame->quality.value();
    double brightness = flame->brightness.value();
    double contrast = flame->contrast.value();
    double ref = 1.0*quality*area/(contrast*scale*scale);

    params.a = contrast*brightness*268.0/256;
    params.b = 1.0/ref;
}

shared_ptr<clwrap::CLEvent> ToneMapper::runAsync(ToneMapperParams& params, vector<float>& hist) {
    aArg.set(params.a);
    bArg.set(params.b);
    histArg.lazy(hist);
    chunkArg.set(ceil(1.0*params.width*params.height)/GLOBAL_WORK_SIZE);
    return kernel.runAsync(GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);
}

void ToneMapper::read(shared_ptr<clwrap::CLEvent> event, vector<float>& imageData) {
    return histArg.getAfterEvent(event, imageData);
}

}