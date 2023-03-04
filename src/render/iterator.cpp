#include "iterator.hpp"
#include "cl_context.hpp"
#include "cl_queue.hpp"
#include "palette.hpp"
#include <random>

using std::vector;

namespace render {

Iterator::Iterator(const CLQueuedContext& context_, Flame flame, int globalWorkSize_,
    int localWorkSize_, int initialIters):
    context(context_),
    kernel(context, "iterate", "src/render/cl/iterate.cl"),
    globalWorkSize(globalWorkSize_),
    localWorkSize(localWorkSize_),
    flameCL(kernel, 0, flame.getFlameCL()),
    stateArg(kernel, 1,
        [flame, globalWorkSize_] (auto& arr) {
            flame.readInitialStateArray(arr, globalWorkSize_);
        }
    ),
    xformArg(kernel, 2,
        [flame] (auto& arr) {
            flame.readXFormCLArray(arr);
        }
    ),
    xformDistArg(kernel, 3,
        [flame] (auto& arr) {
            flame.readXFormDistribution(arr);
        }
    ),
    paletteArg(kernel, 4,
        [flame] (auto& arr) {
            flame.palette.readColorCLArray(arr);
        }
    ),
    outputArg(kernel, 5, 4*globalWorkSize)
{
    for (int i=0; i<initialIters; i++) {
        kernel.run(globalWorkSize, localWorkSize);
    }
}

void Iterator::run() {
    kernel.run(globalWorkSize, localWorkSize);
}

void Iterator::readOutput(vector<float>& arr) {
    outputArg.buffer.read(arr);
}

}