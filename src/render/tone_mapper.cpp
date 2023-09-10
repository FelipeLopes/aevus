#include "tone_mapper.hpp"

using clwrap::CLQueuedContext;
using std::vector;

namespace render {

ToneMapper::ToneMapper(const CLQueuedContext& context_):
    context(context_),
    kernel(context, "mapping", "src/render/cl/mapping.cl"),
    aArg(&kernel, 0, 1.0),
    bArg(&kernel, 1, 1.0),
    histArg(&kernel, 2) { }

void ToneMapper::setParams(int area, float a, float b, vector<float>& hist) {
    aArg.set(a);
    bArg.set(b);
    histArg.set(hist);
    kernel.runBlocking(area, LOCAL_WORK_SIZE);
}

}