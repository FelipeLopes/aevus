#include "renderer.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"

using clwrap::CLQueuedContext;
using core::Flame;
using std::stringstream;

namespace render {

Renderer::Renderer(CLQueuedContext& context, stringstream& stream_): stream(stream_),
    iterator(context, stream), toneMapper(context) { }

void Renderer::renderFlame(Flame* flame_) {
    flame = flame_;
    iterator.setup(flame);
    iterator.runAsync([this] {
        iterator.getRenderData();
        toneMapper.setup(flame, iterator.renderData);
        toneMapper.runAsync([this] {
            toneMapper.writePNMImage(stream);
        });
    });
}

}