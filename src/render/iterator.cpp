#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <random>

using core::Flame;
using std::shared_ptr;
using std::stringstream;
using std::vector;

using clwrap::CLQueuedContext;

namespace render {

Iterator::Iterator(CLQueuedContext& context_, stringstream& out_):
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
    flame->readInitialStateArray(stateArg.argVec, GLOBAL_WORK_SIZE);
    stateArg.lazy();
    flame->readXFormCLArray(xformArg.argVec);
    xformArg.lazy();
    flame->readXFormDistribution(xformDistArg.argVec);
    xformDistArg.lazy();
    flame->palette.readColorCLArray(paletteArg.argVec);
    paletteArg.lazy();
    histogramArg.argVec.resize(4*area);
    std::fill(histogramArg.argVec.begin(), histogramArg.argVec.end(), 0.0f);
    histogramArg.lazy();
    double samples = area*quality;
    itersArg.set(ceil(samples/GLOBAL_WORK_SIZE));
}

void Iterator::runAsync(std::function<void(shared_ptr<vector<float>>)> block) {
    auto execEvent = kernel.runAsync(GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);
    auto histogram = std::make_shared<vector<float>>();
    auto readEvent = histogramArg.getAsyncAfterEvent(execEvent, histogram);
    context.setEventCallback(readEvent, clwrap::CLEvent::COMPLETE, [histogram, block] {
        block(histogram);
    });
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