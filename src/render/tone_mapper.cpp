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

void ToneMapper::setup(Flame* flame, std::shared_ptr<vector<float>> hist) {
    auto sz = flame->size.value();
    width = sz.width;
    height = sz.height;
    background = flame->background.value().toColorCL();
    int area = width * height;
    double scale = flame->scale.value();
    int quality = flame->quality.value();
    double brightness = flame->brightness.value();
    double scale2 = scale*scale;
    double ref = 1.0*quality*area/scale2;

    double a = brightness*268.0/256;
    double b = 1.0/ref;

    aArg.set(a);
    bArg.set(b);
    histArg.set(*hist);
}

void ToneMapper::runAsync(std::function<void(shared_ptr<vector<float>>)> block) {
    auto execEvent = kernel.runAsync(width*height, LOCAL_WORK_SIZE);
    auto imageData = std::make_shared<vector<float>>();
    histArg.getAsyncAfterEvent(execEvent, [block] (auto readResult) {
        block(readResult.get());
    });
}

}