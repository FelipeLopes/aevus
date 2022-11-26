#include "iterator.hpp"
#include "cl_context.hpp"
#include "cl_queue.hpp"
#include "palette.hpp"
#include "xform_distribution.hpp"

namespace render {

Iterator::Iterator(Flame flame, const CLContext& context_, const CLQueue& queue_):
    context(context_), queue(queue_),
    flameCL(flame.getFlameCL()),
    stateBuf(context, queue, 1024),
    xformBuf(context, queue, flame.xforms.size()),
    xformDistBuf(context, queue, flame.xforms.size()*XFormDistribution::XFORM_DISTRIBUTION_GRAINS),
    paletteBuf(context, queue, Palette::PALETTE_WIDTH),
    outputBuf(context, queue, 1024*2)
{

}

}