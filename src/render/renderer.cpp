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
    iterator.setFlame(flame);
    iterator.render();
    auto sz = flame->size.value();
    int area = sz.width * sz.height;
    double scale = flame->scale.value();
    int quality = flame->quality.value();
    double brightness = flame->brightness.value();
    double scale2 = scale*scale;
    double ref = 1.0*quality*area/scale2;
    toneMapper.setParams(area, brightness*268.0/256, 1.0/ref, iterator.renderData);
}

}