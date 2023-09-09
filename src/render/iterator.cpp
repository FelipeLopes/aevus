#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <random>

using core::Flame;
using std::stringstream;
using std::vector;

using clwrap::CLQueuedContext;

namespace render {

Iterator::Iterator(const CLQueuedContext& context_, Flame* flame, stringstream& out):
    context(context_),
    kernel(context, "iterate", "src/render/cl/iterate.cl"),
    width(flame->size.value().width),
    height(flame->size.value().height),
    scale(flame->scale.value()),
    quality(flame->quality.value()),
    brightness(flame->brightness.value()),
    background(flame->background.value().toColorCL()),
    flameCL(kernel, 0, flame->getFlameCL()),
    stateArg(kernel, 1,
        [&flame] (auto& arr) {
            flame->readInitialStateArray(arr, GLOBAL_WORK_SIZE);
        }
    ),
    xformArg(kernel, 2,
        [&flame] (auto& arr) {
            flame->readXFormCLArray(arr);
        }
    ),
    xformDistArg(kernel, 3,
        [&flame] (auto& arr) {
            flame->readXFormDistribution(arr);
        }
    ),
    paletteArg(kernel, 4,
        [&flame] (auto& arr) {
            flame->palette.readColorCLArray(arr);
        }
    ),
    histogramArg(kernel, 5, 4*width*height)
{
    int area = width*height;
    vector<float> zeros;
    zeros.resize(4*area);
    fill(zeros.begin(), zeros.end(), 0);
    histogramArg.buffer.write(zeros);
    /*int samples = width*height*quality;
    for (int i=0; i<samples/GLOBAL_WORK_SIZE; i++) {
        kernel.runBlocking(GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);
    }
    kernel.runBlocking(samples%GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);*/
    vector<float> arr;
    histogramArg.buffer.read(arr);
    double scale2 = ((double)scale)*scale;
    double ref = 1.0*quality*area/scale2;
    ToneMapper toneMapper(context, area, brightness*268.0/256, 1.0/ref, arr);
    toneMapper.readOutput(arr);
    writePNMImage(out, arr);
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