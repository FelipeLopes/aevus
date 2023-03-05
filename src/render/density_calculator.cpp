#include "density_calculator.hpp"

namespace render {

DensityCalculator::DensityCalculator(const CLQueuedContext& context_, float scale, float slope,
    std::vector<float>& hist):
    context(context_),
    kernel(context, "density", "src/render/cl/density.cl"),
    scaleArg(kernel, 0, scale),
    slopeArg(kernel, 1, slope),
    histArg(kernel, 2, hist)
{

}

}