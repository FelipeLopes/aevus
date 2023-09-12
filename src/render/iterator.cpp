#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <random>

using core::Flame;
using std::stringstream;
using std::vector;

using clwrap::CLQueuedContext;

namespace render {

Iterator::Iterator(const CLQueuedContext& context_, stringstream& out_):
    context(context_),
    out(out_),
    kernel(context, "iterate"),
    flameArg(&kernel, 0, core::FlameCL()),
    stateArg(&kernel, 1),
    xformArg(&kernel, 2),
    xformDistArg(&kernel, 3),
    paletteArg(&kernel, 4),
    histogramArg(&kernel, 5),
    itersArg(&kernel, 6, 0) { }

void Iterator::setup(Flame* flame) {
    auto sz = flame->size.value();
    width = sz.width;
    height = sz.height;
    scale = flame->scale.value();
    quality = flame->quality.value();
    brightness = flame->brightness.value();
    background = flame->background.value().toColorCL();

    int area = width*height;
    flameArg.set(flame->getFlameCL());
    vector<core::IterationState> stateVec;
    flame->readInitialStateArray(stateVec, GLOBAL_WORK_SIZE);
    stateArg.set(stateVec);
    vector<core::XFormCL> xformVec;
    flame->readXFormCLArray(xformVec);
    xformArg.set(xformVec);
    vector<uint8_t> xformDistVec;
    flame->readXFormDistribution(xformDistVec);
    xformDistArg.set(xformDistVec);
    vector<core::ColorCL> paletteVec;
    flame->palette.readColorCLArray(paletteVec);
    paletteArg.set(paletteVec);
    vector<float> hist;
    hist.resize(4*area);
    std::fill(hist.begin(), hist.end(), 0.0f);
    histogramArg.set(hist);
    double samples = area*quality;
    itersArg.set(ceil(samples/GLOBAL_WORK_SIZE));
}

void Iterator::run() {
    kernel.runBlocking(GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);
    renderData.resize(4*width*height);
    histogramArg.get(renderData);
}

void Iterator::runAsync(Renderer* renderer, void (*block)(Renderer*)) {
    auto event = kernel.runAsync(GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);
    event->setCallback(renderer, clwrap::CLEvent::COMPLETE, block);
}

void Iterator::getRenderData() {
    renderData.resize(4*width*height);
    histogramArg.get(renderData);
}

void Iterator::writePNMImage() {
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

void Iterator::writePAMImage(stringstream& out, vector<float>& arr) {
    out << "P7\nWIDTH " << width <<
        "\nHEIGHT "<< height <<
        "\nDEPTH 4\nMAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";
    float bg_ra = background.r * background.a;
    float bg_ga = background.g * background.a;
    float bg_ba = background.b * background.a;
    for (int i=0; i<arr.size()/4; i++) {
        float a = arr[4*i+3];
        a = std::min(a, 1.0f);
        float r = arr[4*i]*a + bg_ra*(1-a);
        float g = arr[4*i+1]*a + bg_ga*(1-a);
        float b = arr[4*i+2]*a + bg_ba*(1-a);

        float af = a + background.a - a*background.a;

        r /= af;
        g /= af;
        b /= af;

        out.put((uint8_t)(r*255));
        out.put((uint8_t)(g*255));
        out.put((uint8_t)(b*255));
        out.put((uint8_t)(af*255));
    }
}

}