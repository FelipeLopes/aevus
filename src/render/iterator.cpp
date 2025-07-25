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
    paramArg(&kernel, 4),
    xformDistArg(&kernel, 5),
    paletteArg(&kernel, 6),
    histogramArg(&kernel, 7),
    posFinalXFormArg(&kernel, 8, -1),
    thresholdArg(&kernel, 9, 0.0f),
    itersArg(&kernel, 10, 0) { }

void Iterator::extractParams(Flame* flame, IteratorParams& params) {
    params.flameCL = flame->getFlameCL();
    flame->readInitialStateArray(params.stateVec, GLOBAL_WORK_SIZE);
    flame->readXFormData(params.xformVec, params.varVec, params.paramVec);
    flame->readXFormDistribution(params.xformDistVec);
    flame->palette.readColorCLArray(params.paletteVec);
    params.posFinalXForm = flame->readFinalXFormPosition();
    double samples = flame->quality.value()*params.flameCL.width*params.flameCL.height;
    params.iters = ceil(samples/GLOBAL_WORK_SIZE);
}

std::shared_ptr<clwrap::CLEvent> Iterator::runAsync(IteratorParams& params) {
    int histSize = 4*ceil(1.0*params.flameCL.width*params.flameCL.height/GLOBAL_WORK_SIZE) *
        GLOBAL_WORK_SIZE;
    histogramVec.resize(histSize);
    std::fill(histogramVec.begin(), histogramVec.end(), 0.0f);
    if (params.xformVec.size() == 0) {
        return NULL;
    }
    flameArg.set(params.flameCL);
    stateArg.lazy(params.stateVec);
    xformArg.lazy(params.xformVec);
    varArg.lazy(params.varVec);
    paramArg.lazy(params.paramVec);
    xformDistArg.lazy(params.xformDistVec);
    paletteArg.lazy(params.paletteVec);
    histogramArg.lazy(histogramVec);
    posFinalXFormArg.set(params.posFinalXForm);
    thresholdArg.set(params.threshold);
    itersArg.set(params.iters);
    return kernel.runAsync(GLOBAL_WORK_SIZE, LOCAL_WORK_SIZE);
}

void Iterator::read(std::shared_ptr<clwrap::CLEvent> event, vector<float>& histogram) {
    if (event != NULL) {
        histogramArg.getAfterEvent(event, histogram);
    } else {
        histogram = histogramVec;
    }
}

void Iterator::writePAMImage(stringstream& out, vector<float>& arr) {
    /*out << "P7\nWIDTH " << width <<
        "\nHEIGHT "<< height <<
        "\nDEPTH 4\nMAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";*/
}

}