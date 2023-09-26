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
    int quality = flame->quality.value();
    double brightness = flame->brightness.value();
    double scale2 = scale*scale;
    double ref = 1.0*quality*area/scale2;

    params.a = brightness*268.0/256;
    params.b = 1.0/ref;
}

shared_ptr<clwrap::CLEvent> ToneMapper::runAsync(ToneMapperParams& params, vector<float>& hist) {
    aArg.set(params.a);
    bArg.set(params.b);
    histArg.lazy(hist);
    chunkArg.set(ceil(1.0*params.width*params.height)/GLOBAL_WORK_SIZE);
    return kernel.runAsync(GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);
}

void ToneMapper::readAsync(shared_ptr<clwrap::CLEvent> event,
    std::function<void (std::shared_ptr<std::vector<float>>)> block)
{
    histArg.getAsyncAfterEvent(event, [block] (auto readResult) {
        block(readResult.get());
    });
}

}