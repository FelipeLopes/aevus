#include "tone_mapper.hpp"

namespace render {

ToneMapper::ToneMapper(const CLQueuedContext& context_, int area_, float a, float b,
    std::vector<float>& hist):
    context(context_),
    area(area_),
    kernel(context, "mapping", "src/render/cl/mapping.cl"),
    aArg(kernel, 0, a),
    bArg(kernel, 1, b),
    histArg(kernel, 2, hist)
{
    kernel.runBlocking(area, LOCAL_WORK_SIZE);
}

void ToneMapper::readOutput(std::vector<float> &arr) {
    histArg.buffer.read(arr);
}

}