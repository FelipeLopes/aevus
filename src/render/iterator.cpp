#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <random>

using core::Flame;
using std::stringstream;
using std::vector;

using clwrap::CLQueuedContext;

namespace render {

Iterator::Iterator(const CLQueuedContext& context_, stringstream& out):
    context(context_),
    kernel(context, "iterate", "src/render/cl/iterate.cl"),
    flameArg(&kernel, 0, core::FlameCL()),
    stateArg(&kernel, 1),
    xformArg(&kernel, 2),
    xformDistArg(&kernel, 3),
    paletteArg(&kernel, 4),
    histogramArg(&kernel, 5)
{
    /*int samples = width*height*quality;
    for (int i=0; i<samples/GLOBAL_WORK_SIZE; i++) {
        kernel.runBlocking(GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);
    }
    kernel.runBlocking(samples%GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);*/
    vector<float> arr;
    arr.resize(4*width*height);
    //histogramArg.get(arr);
    /*double scale2 = ((double)scale)*scale;
    double ref = 1.0*quality*area/scale2;
    ToneMapper toneMapper(context, area, brightness*268.0/256, 1.0/ref, arr);
    toneMapper.readOutput(arr);*/
    writePNMImage(out, arr);
}

void Iterator::setFlame(Flame* flame) {
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

void Iterator::writePNMImage(stringstream& out, vector<float>& arr) {
    out << "P6\n" << width << " " << height << "\n255\n";
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
    }
}

}