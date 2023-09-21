#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <memory>
#include <random>
#include <vector>

using core::Flame;
using std::shared_ptr;
using std::stringstream;
using std::vector;

using clwrap::CLQueuedContext;

namespace render {

Iterator::Iterator(CLQueuedContext& context_):
    context(context_),
    kernel(context, "iterate"),
    flameArg(&kernel, 0, core::FlameCL()),
    stateArg(&kernel, 1),
    xformArg(&kernel, 2),
    varArg(&kernel, 3),
    xformDistArg(&kernel, 4),
    paletteArg(&kernel, 5),
    histogramArg(&kernel, 6),
    itersArg(&kernel, 7, 0) { }

void Iterator::extractParams(Flame* flame, IteratorParams& params) {
    params.flameCL = flame->getFlameCL();
    flame->readInitialStateArray(params.stateVec, GLOBAL_WORK_SIZE);
    flame->readXFormData(params.xformVec, params.varVec);
    flame->readXFormDistribution(params.xformDistVec);
    flame->palette.readColorCLArray(params.paletteVec);
    double samples = flame->quality.value()*params.flameCL.width*params.flameCL.height;
    params.iters = ceil(samples/GLOBAL_WORK_SIZE);
}

void Iterator::runAsync(IteratorParams& params,
    std::function<void(shared_ptr<vector<float>>)> block)
{
    if (params.xformVec.size() == 0) {
        auto arr = std::make_shared<vector<float>>();
        arr->resize(4*params.flameCL.width*params.flameCL.height);
        std::fill(arr->begin(), arr->end(), 0.0f);
        block(arr);
        return;
    }
    flameArg.set(params.flameCL);
    stateArg.lazy(params.stateVec);
    xformArg.lazy(params.xformVec);
    varArg.lazy(params.varVec);
    xformDistArg.lazy(params.xformDistVec);
    paletteArg.lazy(params.paletteVec);
    vector<float> histogramVec;
    histogramVec.resize(4*params.flameCL.width*params.flameCL.height);
    std::fill(histogramVec.begin(), histogramVec.end(), 0.0f);
    histogramArg.lazy(histogramVec);
    itersArg.set(params.iters);
    auto execEvent = kernel.runAsync(GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);
    auto histogram = std::make_shared<vector<float>>();
    histogramArg.getAsyncAfterEvent(execEvent, [block] (auto readResult) {
        block(readResult.get());
    });
}

void Iterator::writePAMImage(stringstream& out, vector<float>& arr) {
    /*out << "P7\nWIDTH " << width <<
        "\nHEIGHT "<< height <<
        "\nDEPTH 4\nMAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";*/
}

}