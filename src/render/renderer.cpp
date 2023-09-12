#include "renderer.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"

using clwrap::CLQueuedContext;
using core::Flame;
using std::stringstream;

namespace render {

Renderer::Renderer(const CLQueuedContext& context, stringstream& stream_): stream(stream_),
    iterator(context, stream), toneMapper(context) { }

void Renderer::renderFlame(Flame* flame_) {
    flame = flame_;
    iterator.setup(flame);
    iterator.runAsync(this, [](auto r) {
        r->iterator.getRenderData();
        r->toneMapper.setup(r->flame, r->iterator.renderData);
        r->toneMapper.runAsync(r, [](auto r) {
            r->toneMapper.writePNMImage(r->stream);
        });
    });
}

}