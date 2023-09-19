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
    histArg(&kernel, 2) { }

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

void ToneMapper::runAsync(ToneMapperParams& params, shared_ptr<vector<float>> hist,
    std::function<void(shared_ptr<vector<float>>)> block)
{
    aArg.set(params.a);
    bArg.set(params.b);
    histArg.lazy(*hist);
    auto execEvent = kernel.runAsync(params.width*params.height, LOCAL_WORK_SIZE);
    auto imageData = std::make_shared<vector<float>>();
    histArg.getAsyncAfterEvent(execEvent, [block] (auto readResult) {
        block(readResult.get());
    });
}

}