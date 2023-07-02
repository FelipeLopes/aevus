#include "tone_mapper.hpp"

namespace render {

ToneMapper::ToneMapper(const CLQueuedContext& context_, float scale, float slope,
    std::vector<float>& hist):
    context(context_),
    kernel(context, "mapping", "src/render/cl/mapping.cl"),
    scaleArg(kernel, 0, scale),
    slopeArg(kernel, 1, slope),
    histArg(kernel, 2, hist)
{

}

}