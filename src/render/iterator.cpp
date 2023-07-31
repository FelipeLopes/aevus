#include "iterator.hpp"
#include "cl_context.hpp"
#include "cl_queue.hpp"
#include "tone_mapper.hpp"
#include "palette.hpp"
#include <random>

using std::shared_ptr;
using std::vector;

namespace render {

Iterator::Iterator(const CLQueuedContext& context_, shared_ptr<core::Flame> flame):
    context(context_),
    kernel(context, "iterate", "src/render/cl/iterate.cl"),
    width(flame->size.getValue().width),
    height(flame->size.getValue().height),
    scale(flame->scale.getValue()),
    quality(flame->quality.getValue()),
    brightness(flame->brightness.getValue()),
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
    writeImage("../square.pam", arr);
}

void Iterator::writeImage(std::string filename, std::vector<float>& arr) {
    std::ostringstream os;
    os << "P7\nWIDTH " << width <<
        "\nHEIGHT "<< height <<
        "\nDEPTH 4\nMAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";
    FILE* f = fopen(filename.c_str(),"wb");
    std::string s = os.str();
    fwrite(s.c_str(), 1, s.size(), f);
    for (int i=0; i<arr.size(); i++) {
        uint8_t val = (arr[i] > 1.0 || i%4 == 3) ? 255 : (uint8_t)(arr[i]*255);
        fputc(val,f);
    }
}

}