#include "renderer.hpp"
#include "iterator.hpp"
#include "tone_mapper.hpp"

using clwrap::CLQueuedContext;
using core::ColorCL;
using core::Flame;
using std::stringstream;
using std::vector;

namespace render {

Renderer::Renderer(CLQueuedContext& context, Flame* flame_, stringstream& stream_): stream(stream_),
    flame(flame_), iterator(context), toneMapper(context) { }

void Renderer::renderFlame() {
    iterator.setup(flame);
    iterator.runAsync([this] (auto hist) {
        toneMapper.setup(flame, hist);
        toneMapper.runAsync([this] (auto imgData) {
            auto sz = flame->size.value();
            writePNMImage(*imgData, sz.width, sz.height, flame->background.value().toColorCL());
            imageRendered();
        });
    });
}

void Renderer::writePNMImage(vector<float>& imgData, int width, int height, ColorCL background) {
    stream.str("");
    stream.clear();
    stream << "P6\n" << width << " " << height << "\n255\n";
    float bg_ra = background.r * background.a;
    float bg_ga = background.g * background.a;
    float bg_ba = background.b * background.a;
    for (int i=0; i<imgData.size()/4; i++) {
        float a = imgData[4*i+3];
        a = std::min(a, 1.0f);
        float r = imgData[4*i]*a + bg_ra*(1-a);
        float g = imgData[4*i+1]*a + bg_ga*(1-a);
        float b = imgData[4*i+2]*a + bg_ba*(1-a);
        float af = a + background.a - a*background.a;

        r /= af;
        g /= af;
        b /= af;

        stream.put((uint8_t)(r*255));
        stream.put((uint8_t)(g*255));
        stream.put((uint8_t)(b*255));
    }
}

}