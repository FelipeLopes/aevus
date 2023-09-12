#include "tone_mapper.hpp"

using clwrap::CLQueuedContext;
using core::Flame;
using std::stringstream;
using std::vector;

namespace render {

ToneMapper::ToneMapper(const CLQueuedContext& context_):
    context(context_),
    kernel(context, "mapping"),
    aArg(&kernel, 0, 1.0),
    bArg(&kernel, 1, 1.0),
    histArg(&kernel, 2) { }

void ToneMapper::setup(Flame* flame, vector<float>& hist) {
    auto sz = flame->size.value();
    width = sz.width;
    height = sz.height;
    background = flame->background.value().toColorCL();
    int area = width * height;
    double scale = flame->scale.value();
    int quality = flame->quality.value();
    double brightness = flame->brightness.value();
    double scale2 = scale*scale;
    double ref = 1.0*quality*area/scale2;

    double a = brightness*268.0/256;
    double b = 1.0/ref;

    aArg.set(a);
    bArg.set(b);
    histArg.set(hist);
}

void ToneMapper::runAsync(Renderer* renderer, void (*block)(Renderer *)) {
    auto event = kernel.runAsync(width*height, LOCAL_WORK_SIZE);
    event->setCallback<Renderer>(renderer, clwrap::CLEvent::COMPLETE, block);
}

void ToneMapper::writePNMImage(stringstream& out) {
    vector<float> renderData;
    histArg.get(renderData);
    out.str("");
    out.clear();
    out << "P6\n" << width << " " << height << "\n255\n";
    float bg_ra = background.r * background.a;
    float bg_ga = background.g * background.a;
    float bg_ba = background.b * background.a;
    for (int i=0; i<renderData.size()/4; i++) {
        float a = renderData[4*i+3];
        a = std::min(a, 1.0f);
        float r = renderData[4*i]*a + bg_ra*(1-a);
        float g = renderData[4*i+1]*a + bg_ga*(1-a);
        float b = renderData[4*i+2]*a + bg_ba*(1-a);
        float af = a + background.a - a*background.a;

        r /= af;
        g /= af;
        b /= af;

        out.put((uint8_t)(r*255));
        out.put((uint8_t)(g*255));
        out.put((uint8_t)(b*255));
    }
}

}