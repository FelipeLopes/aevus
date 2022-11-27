#include "iterator.hpp"
#include "cl_context.hpp"
#include "cl_queue.hpp"
#include "palette.hpp"
#include "xform_distribution.hpp"
#include <random>

namespace render {

Iterator::Iterator(Flame flame, const CLQueuedContext& context_,
    std::vector<IterationState>& stateVec, std::vector<XFormCL>& xformVec,
    std::vector<uint8_t>& xformDistVec, std::vector<ColorCL>& paletteVec):
    context(context_),
    kernel(context, "iterate", "src/render/cl/iterate.cl"),
    flameCL(kernel, 0, flame.getFlameCL()),
    stateArg(kernel, 1, stateVec),
    xformArg(kernel, 2, xformVec),
    xformDistArg(kernel, 3, xformDistVec),
    paletteArg(kernel, 4, paletteVec),
    outputArg(kernel, 5, 1024*2)
{
    for (int i=0; i<20; i++) {
        kernel.run(1024, 64);
    }
}

void Iterator::run() {
    kernel.run(1024, 64);
}

void Iterator::readOutput(std::vector<float>& arr) {
    outputArg.buffer.read(arr);
}

}