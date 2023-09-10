#include "renderer.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"

using clwrap::CLQueuedContext;
using std::stringstream;

namespace render {

Renderer::Renderer(const CLQueuedContext& context, stringstream& stream_): stream(stream_),
    dummy({0}), iterator(context, stream), toneMapper(context, 64, 1.0, 1.0, dummy) { }

}