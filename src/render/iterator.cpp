#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <memory>
#include <random>
#include <vector>

using core::FlameContent;
using core::FrameContent;
using core::FrameCL;
using core::IterationState;
using std::shared_ptr;
using std::stringstream;
using std::vector;

using clwrap::CLQueuedContext;

namespace render {

Iterator::Iterator(CLQueuedContext& context_):
    context(context_),
    kernel(context, "iterate"),
    frameArg(&kernel, 0, core::FrameCL()),
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

void Iterator::extractParams(const FlameContent& flame, IteratorParams& params) {
    params.frame = getFrame(flame.frame);
    readInitialStateArray(flame, params.stateVec, GLOBAL_WORK_SIZE);
    /*
    flame->readXFormData(params.xformVec, params.varVec, params.paramVec);
    flame->readXFormDistribution(params.xformDistVec);
    flame->palette.readColorCLArray(params.paletteVec);
    params.posFinalXForm = flame->readFinalXFormPosition();
    double samples = flame->quality.value()*params.frame.width*params.frame.height;
    params.iters = ceil(samples/GLOBAL_WORK_SIZE);*/
}

std::shared_ptr<clwrap::CLEvent> Iterator::runAsync(IteratorParams& params) {
    int histSize = 4*ceil(1.0*params.frame.width*params.frame.height/GLOBAL_WORK_SIZE) *
        GLOBAL_WORK_SIZE;
    histogramVec.resize(histSize);
    std::fill(histogramVec.begin(), histogramVec.end(), 0.0f);
    if (params.xformVec.size() == 0) {
        return NULL;
    }
    frameArg.set(params.frame);
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

FrameCL Iterator::getFrame(FrameContent frame) {
    FrameCL frameCL;
    frameCL.cx = frame.flameCenter.x;
    frameCL.cy = frame.flameCenter.y;
    frameCL.scale = frame.flameScale;
    frameCL.width = frame.flameSize.width;
    frameCL.height = frame.flameSize.height;
    return frameCL;
}

void Iterator::readInitialStateArray(const FlameContent& flame, vector<IterationState> &arr, int size) const {
    arr.clear();
    if (flame.xforms.size() == 0) {
        return;
    }
    std::mt19937_64 rng(314159);
    std::uniform_int_distribution<uint64_t> seedDist;
    std::uniform_int_distribution<uint8_t> xformDist(0,flame.xforms.size()-1);
    std::uniform_real_distribution<float> posDist(-1.0, 1.0);
    std::uniform_real_distribution<float> colorDist(0.0, 1.0);
    for (int i = 0; i < size; i++) {
        IterationState st;
        st.x = posDist(rng);
        st.y = posDist(rng);
        st.c = colorDist(rng);
        st.seed.value = seedDist(rng);
        st.xf = xformDist(rng);
        arr.push_back(st);
    }
}

}