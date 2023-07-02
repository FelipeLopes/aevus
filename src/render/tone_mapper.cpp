#include "tone_mapper.hpp"

namespace render {

ToneMapper::ToneMapper(const CLQueuedContext& context_, float a, float b,
    std::vector<float>& hist):
    context(context_),
    kernel(context, "mapping", "src/render/cl/mapping.cl"),
    aArg(kernel, 0, a),
    bArg(kernel, 1, b),
    histArg(kernel, 2, hist)
{

}

}