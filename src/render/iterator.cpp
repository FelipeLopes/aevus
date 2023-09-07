#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <random>

using core::Flame;
using std::vector;

using clwrap::CLQueuedContext;

namespace render {

Iterator::Iterator(const CLQueuedContext& context_, Flame* flame):
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
    int samples = width*height*quality;
    for (int i=0; i<samples/GLOBAL_WORK_SIZE; i++) {
        kernel.runBlocking(GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);
    }
    kernel.runBlocking(samples%GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);
    vector<float> arr;
    histogramArg.buffer.read(arr);
    double scale2 = ((double)scale)*scale;
    double ref = 1.0*quality*area/scale2;
    ToneMapper toneMapper(context, area, brightness*268.0/256, 1.0/ref, arr);
    toneMapper.readOutput(arr);
    writePNMImage("../flame.pnm", arr);
}

void Iterator::writePAMImage(std::string filename, std::vector<float>& arr) {
    std::ostringstream os;
    os << "P7\nWIDTH " << width <<
        "\nHEIGHT "<< height <<
        "\nDEPTH 4\nMAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";
    FILE* f = fopen(filename.c_str(),"wb");
    std::string s = os.str();
    fwrite(s.c_str(), 1, s.size(), f);
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

        fputc((uint8_t)(r*255),f);
        fputc((uint8_t)(g*255),f);
        fputc((uint8_t)(b*255),f);
        fputc((uint8_t)(af*255),f);
    }
    fclose(f);
}

void Iterator::writePNMImage(std::string filename, std::vector<float>& arr) {
    FILE* f = fopen(filename.c_str(),"w");
    fprintf(f, "P6\n3 2\n255\n");
    std::vector<float> fakeArr = {
        1.0, 0.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 1.0,
        1.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0,
        0.0, 0.0, 0.0, 1.0
    };
    for (int i=0; i<fakeArr.size()/4; i++) {
        uint8_t r = fakeArr[4*i]*255;
        uint8_t g = fakeArr[4*i+1]*255;
        uint8_t b = fakeArr[4*i+2]*255;
        fputc(r,f);
        fputc(g,f);
        fputc(b,f);
    }
    fclose(f);
}

}