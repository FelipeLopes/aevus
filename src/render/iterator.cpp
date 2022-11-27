#include "iterator.hpp"
#include "cl_context.hpp"
#include "cl_queue.hpp"
#include "palette.hpp"
#include "xform_distribution.hpp"
#include <random>

using std::vector;

namespace render {

Iterator::Iterator(const CLQueuedContext& context_, Flame flame, FlameCL flameCL_,
    vector<IterationState>& stateVec,
    vector<uint8_t>& xformDistVec, vector<ColorCL>& paletteVec):
    context(context_),
    kernel(context, "iterate", "src/render/cl/iterate.cl"),
    flameCL(kernel, 0, flame.getFlameCL()),
    stateArg(kernel, 1, stateVec),
    xformArg(kernel, 2,
        [flame] (vector<XFormCL>& arr) {
            flame.readXFormCLArray(arr);
        }
    ),
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

void Iterator::readOutput(vector<float>& arr) {
    outputArg.buffer.read(arr);
}

}