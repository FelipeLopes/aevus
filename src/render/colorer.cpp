#include "colorer.hpp"

using clwrap::CLQueuedContext;

namespace render {

Colorer::Colorer(CLQueuedContext& context_):
    context(context_),
    kernel(context, "coloring"),
    densityArg(&kernel, 0),
    backgroundArg(&kernel, 1, core::ColorCL()),
    invGammaArg(&kernel, 2, 1.0f),
    modeArg(&kernel, 3, core::WHITE),
    chunkArg(&kernel, 4, 0),
    imageArg(&kernel, 5) { }

void Colorer::extractParams(core::Flame* flame, ColorerParams& params) {

}

}