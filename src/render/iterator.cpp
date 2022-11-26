#include "iterator.hpp"
#include "cl_context.hpp"
#include "cl_queue.hpp"
#include "palette.hpp"
#include "xform_distribution.hpp"
#include <random>

namespace render {

Iterator::Iterator(Flame flame, const CLContext& context_, const CLQueue& queue_):
    context(context_), queue(queue_),
    kernel("iterate", context.context, context.deviceId, "src/render/cl/iterate.cl"),
    flameCL(flame.getFlameCL()),
    stateBuf(context, queue, 1024),
    xformBuf(context, queue, flame.xforms.size()),
    xformDistBuf(context, queue, flame.xforms.size()*XFormDistribution::XFORM_DISTRIBUTION_GRAINS),
    paletteBuf(context, queue, Palette::PALETTE_WIDTH),
    outputBuf(context, queue, 1024*2)
{
    std::vector<XFormCL> xformVec;
    flame.readXFormCLArray(xformVec);

    std::vector<ColorCL> paletteVec;
    flame.palette.readColorCLArray(paletteVec);

    std::vector<IterationState> stateVec;
    std::mt19937_64 rng(314159);
    std::uniform_int_distribution<uint64_t> dist;
    std::uniform_real_distribution<float> floatDist(-1.0, 1.0);
    for (int i = 0; i < 1024; i++) {
        IterationState st;
        st.x = floatDist(rng);
        st.y = floatDist(rng);
        st.seed.value = dist(rng);
        stateVec.push_back(st);
    }

    stateBuf.write(stateVec);
    xformBuf.write(xformVec);
    paletteBuf.write(paletteVec);

    XFormDistribution distrib;
    flame.readXFormDistribution(distrib);

    kernel.setArg(0, flameCL);
    kernel.setBufferArg(1, stateBuf);
    kernel.setBufferArg(2, xformBuf);
    kernel.setBufferArg(3, xformDistBuf);
    kernel.setBufferArg(4, paletteBuf);
    kernel.setBufferArg(5, outputBuf);

    for (int i=0; i<20; i++) {
        kernel.run(queue, 1024, 64);
    }
}

void Iterator::run() {
    kernel.run(queue, 1024, 64);
}

void Iterator::readOutput(std::vector<float>& arr) {
    outputBuf.read(arr);
}

}