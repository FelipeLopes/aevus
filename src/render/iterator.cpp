#include "iterator.hpp"
#include "tone_mapper.hpp"
#include <memory>
#include <random>
#include <vector>

using core::FlameContent;
using core::FrameContent;
using core::XFormContent;
using core::FrameCL;
using core::XFormCL;
using core::VariationCL;
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
    readXFormData(flame, params.xformVec, params.varVec, params.paramVec);
    readXFormDistribution(flame, params.xformDistVec);
    /*
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

void Iterator::readInitialStateArray(const FlameContent& flame,
    vector<IterationState> &arr, int size) const
{
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

void Iterator::readXFormData(const FlameContent& flame, vector<XFormCL>& xformVec,
    vector<VariationCL>& varVec, vector<float>& paramVec) const
{
    xformVec.resize(flame.xforms.size());
    varVec.clear();
    paramVec.clear();
    int varBegin = 0;
    for (int i=0; i<flame.xforms.size(); i++) {
        xformVec[i] = getXFormCL(flame.xforms[i], varBegin);
        readVariationData(flame.xforms[i], varVec, paramVec);
        varBegin = xformVec[i].varEnd;
    }
    if (flame.finalXForm.has_value()) {
        xformVec.push_back(getXFormCL(flame.finalXForm.value(), varBegin));
        readVariationData(flame.finalXForm.value(), varVec, paramVec);
    }
}

XFormCL Iterator::getXFormCL(XFormContent xform, int varBegin) const {
    XFormCL xf;

    auto pre = xform.preCoefs;

    xf.a = pre.xx;
    xf.b = -pre.yx;
    xf.c = pre.ox;
    xf.d = -pre.xy;
    xf.e = pre.yy;
    xf.f = -pre.oy;

    auto pst = xform.postCoefs;

    xf.pa = pst.xx;
    xf.pb = -pst.yx;
    xf.pc = pst.ox;
    xf.pd = -pst.xy;
    xf.pe = pst.yy;
    xf.pf = -pst.oy;

    xf.varBegin = varBegin;
    xf.varEnd = varBegin + xform.variations.size();

    xf.color = xform.color;
    xf.colorSpeed = xform.colorSpeed;

    return xf;
}

void Iterator::readVariationData(XFormContent xform, std::vector<VariationCL>& vars,
    std::vector<float>& params) const
{
    for (auto kv: xform.variations) {
        VariationCL varCL;
        varCL.id = kv.first;
        varCL.weight = kv.second.weight;
        varCL.paramBegin = params.size();
        for (int i=0; i<kv.second.params.size(); i++) {
            params.push_back(kv.second.params[i]);
        }
        vars.push_back(varCL);
    }
}

void Iterator::readXFormDistribution(const FlameContent& flame, std::vector<uint8_t>& dist) const {
    dist.resize(XFORM_DISTRIBUTION_GRAINS*flame.xforms.size());
    for (int i=0; i<flame.xforms.size(); i++) {
        double acc = 0;
        std::vector<double> densities;
        for (int j=0; j<flame.xforms.size(); j++) {
            acc += flame.xforms[i].chaos.getXaos(j) * flame.xforms[j].weight;
            densities.push_back(acc);
        }
        double step = acc / XFORM_DISTRIBUTION_GRAINS;
        int j = 0;
        double curr = 0;
        for (int k=0; k<XFORM_DISTRIBUTION_GRAINS; k++) {
            while (curr >= densities[j] && j<flame.xforms.size()) {
                j++;
            }
            dist[i*XFORM_DISTRIBUTION_GRAINS+k] = j;
            curr += step;
        }
    }
}

}