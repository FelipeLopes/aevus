#include "tone_mapper.hpp"

using clwrap::CLQueuedContext;

namespace render {

ToneMapper::ToneMapper(const CLQueuedContext& context_, int area_, float a, float b,
    std::vector<float>& hist):
    context(context_),
    area(area_),
    kernel(context, "mapping", "src/render/cl/mapping.cl"),
    aArg(&kernel, 0, a),
    bArg(&kernel, 1, b),
    histArg(&kernel, 2, hist)
{
    //kernel.runBlocking(area, LOCAL_WORK_SIZE);
    printf("%d\n", area);
}

void ToneMapper::readOutput(std::vector<float> &arr) {
    histArg.get(arr);
}

}