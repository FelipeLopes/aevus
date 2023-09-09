#include "renderer.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"

using clwrap::CLQueuedContext;

namespace render {

Renderer::Renderer(const CLQueuedContext& context): dummy({0}),
    iterator(context, &flame, stream), toneMapper(context, 64, 1.0, 1.0, dummy) { }

}