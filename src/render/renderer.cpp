#include "renderer.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"

using clwrap::CLQueuedContext;
using core::Flame;
using std::stringstream;

namespace render {

Renderer::Renderer(const CLQueuedContext& context, stringstream& stream_): stream(stream_),
    iterator(context, stream), toneMapper(context) { }

void Renderer::renderFlame(Flame* flame) {
    iterator.setup(flame);
    iterator.run();
    toneMapper.setup(flame, iterator.renderData);
    toneMapper.run();
    toneMapper.writePNMImage(stream);
}

}