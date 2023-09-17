#include "renderer.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"

using clwrap::CLQueuedContext;
using core::Flame;
using std::stringstream;

namespace render {

Renderer::Renderer(CLQueuedContext& context, Flame* flame_, stringstream& stream_): stream(stream_),
    flame(flame_), iterator(context, stream), toneMapper(context) { }

void Renderer::renderFlame() {
    iterator.setup(flame);
    iterator.runAsync([this] (auto hist) {
        toneMapper.setup(flame, hist);
        toneMapper.runAsync([this] {
            toneMapper.writePNMImage(stream);
            imageRendered();
        });
    });
}

}