#include "iterator.hpp"
#include "cl_context.hpp"
#include "cl_queue.hpp"
#include "palette.hpp"
#include <random>

using std::vector;

namespace render {

Iterator::Iterator(const CLQueuedContext& context_, Flame flame, int globalWorkSize_,
    int localWorkSize_, int initialIters, int histIters):
    context(context_),
    kernel(context, "iterate", "src/render/cl/iterate.cl"),
    globalWorkSize(globalWorkSize_),
    localWorkSize(localWorkSize_),
    width(flame.width),
    height(flame.height),
    flameCL(kernel, 0, flame.getFlameCL()),
    stateArg(kernel, 1,
        [flame, globalWorkSize_] (auto& arr) {
            flame.readInitialStateArray(arr, globalWorkSize_);
        }
    ),
    xformArg(kernel, 2,
        [flame] (auto& arr) {
            flame.readXFormCLArray(arr);
        }
    ),
    xformDistArg(kernel, 3,
        [flame] (auto& arr) {
            flame.readXFormDistribution(arr);
        }
    ),
    paletteArg(kernel, 4,
        [flame] (auto& arr) {
            flame.palette.readColorCLArray(arr);
        }
    ),
    histogramArg(kernel, 5, 4*width*height)
{
    for (int i=0; i<initialIters; i++) {
        kernel.run(globalWorkSize, localWorkSize);
    }
    std::vector<float> zeros;
    zeros.resize(4*flame.width*flame.height);
    fill(zeros.begin(), zeros.end(), 0);
    histogramArg.buffer.write(zeros);
    for (int i=0; i<histIters; i++) {
        kernel.run(globalWorkSize, localWorkSize);
    }
}

void Iterator::readHistogram(vector<float>& arr) {
    histogramArg.buffer.read(arr);
}

void Iterator::writeImage(std::string filename) {
    std::ostringstream os;
    os << "P7\nWIDTH " << width <<
        "\nHEIGHT "<< height <<
        "\nDEPTH 4\nMAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";
    FILE* f = fopen(filename.c_str(),"wb");
    std::string s = os.str();
    fwrite(s.c_str(), 1, s.size(), f);
}

}