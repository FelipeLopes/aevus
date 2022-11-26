#include "iterator.hpp"
#include "cl_context.hpp"
#include "cl_queue.hpp"
#include "palette.hpp"
#include "xform_distribution.hpp"
#include <random>

namespace render {

Iterator::Iterator(Flame flame, const CLQueuedContext& context_,
    std::vector<IterationState>& stateVec):
    context(context_), queue(context.defaultQueue),
    kernel(context, "iterate", "src/render/cl/iterate.cl"),
    flameCL(kernel, 0, flame.getFlameCL()),
    state(kernel, 1, stateVec),
    xformBuf(context, queue, flame.xforms.size()),
    xformDistBuf(context, queue, flame.xforms.size()*XFormDistribution::XFORM_DISTRIBUTION_GRAINS),
    paletteBuf(context, queue, Palette::PALETTE_WIDTH),
    outputBuf(context, queue, 1024*2)
{
    std::vector<XFormCL> xformVec;
    flame.readXFormCLArray(xformVec);

    std::vector<ColorCL> paletteVec;
    flame.palette.readColorCLArray(paletteVec);

    xformBuf.write(xformVec);
    paletteBuf.write(paletteVec);

    XFormDistribution distrib;
    flame.readXFormDistribution(distrib);

    kernel.setBufferArg(2, xformBuf);
    kernel.setBufferArg(3, xformDistBuf);
    kernel.setBufferArg(4, paletteBuf);
    kernel.setBufferArg(5, outputBuf);

    for (int i=0; i<20; i++) {
        kernel.run(1024, 64);
    }
}

void Iterator::run() {
    kernel.run(1024, 64);
}

void Iterator::readOutput(std::vector<float>& arr) {
    outputBuf.read(arr);
}

}