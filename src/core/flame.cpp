#include "flame.hpp"
#include <memory>
#include <random>
#include <stdexcept>

using std::string;
using std::vector;

namespace core {

Clipping::Clipping(): mode(WHITE) { }

std::optional<std::string> Clipping::toString() {
    switch(mode) {
        case ALPHA: return "alpha";
        case CHANNEL: return "channel";
        case WHITE: return "white";
    }
}

void Clipping::fromString(std::optional<std::string> text) {
    if (!text.has_value()) {
        mode = WHITE;
    } else if (text == "alpha") {
        mode = ALPHA;
    } else if (text == "channel") {
        mode = CHANNEL;
    } else if (text == "white") {
        mode = WHITE;
    } else {
        throw std::invalid_argument("Unrecognized clipping mode");
    }
}

Flame::Flame(): XMLElementClass("flame"),
    version(*this, "version"),
    name(*this, "name"),
    size(*this, "size"),
    center(*this, "center"),
    scale(*this, "scale"),
    quality(*this, "quality"),
    background(*this, "background"),
    brightness(*this, "brightness"),
    contrast(*this, "contrast", 1.0),
    gamma(*this, "gamma"),
    vibrancy(*this, "vibrancy", 1.0),
    clipping(*this, "clipping"),
    xforms(*this, "xform"),
    finalXForm(*this, "finalxform"),
    palette(*this)
{
    center.get()->x = 0;
    center.get()->y = 0;
    scale.setValue(100);
    size.get()->width = 732;
    size.get()->height = 640;
    quality.setValue(5);
    brightness.setValue(4);
    contrast.setValue(1);
    gamma.setValue(4);
    vibrancy.setValue(1);
}

void Flame::readInitialStateArray(vector<IterationState> &arr, int size) const {
    arr.clear();
    if (xforms.size() == 0) {
        return;
    }
    std::mt19937_64 rng(314159);
    std::uniform_int_distribution<uint64_t> seedDist;
    std::uniform_int_distribution<uint8_t> xformDist(0,xforms.size()-1);
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

void Flame::readXFormData(vector<XFormCL>& xformVec, vector<VariationCL>& varVec,
    vector<float>& paramVec) const
{
    xformVec.resize(xforms.size());
    varVec.clear();
    paramVec.clear();
    int varBegin = 0;
    for (int i=0; i<xforms.size(); i++) {
        xformVec[i] = xforms.get(i)->toXFormCL(varBegin);
        xforms.get(i)->readVariationData(varVec, paramVec);
        varBegin = xformVec[i].varEnd;
    }
    if (finalXForm.isSet()) {
        xformVec.push_back(finalXForm.get()->toXFormCL(varBegin));
        finalXForm.get()->readVariationData(varVec, paramVec);
    }
}

int Flame::readFinalXFormPosition() const {
    if (finalXForm.isSet()) {
        return xforms.size();
    } else {
        return -1;
    }
}

void Flame::readXFormDistribution(std::vector<uint8_t>& dist) const {
    dist.resize(XFORM_DISTRIBUTION_GRAINS*xforms.size());
    for (int i=0; i<xforms.size(); i++) {
        double acc = 0;
        std::vector<double> densities;
        for (int j=0; j<xforms.size(); j++) {
            acc += xforms.get(i)->chaos.value().getXaos(j) *
                xforms.get(j)->weight.value();
            densities.push_back(acc);
        }
        double step = acc / XFORM_DISTRIBUTION_GRAINS;
        int j = 0;
        double curr = 0;
        for (int k=0; k<XFORM_DISTRIBUTION_GRAINS; k++) {
            while (curr >= densities[j] && j<xforms.size()) {
                j++;
            }
            dist[i*XFORM_DISTRIBUTION_GRAINS+k] = j;
            curr += step;
        }
    }
}

tinyxml2::XMLNode* Flame::nodeDeserialize(tinyxml2::XMLNode* node) {
    auto element = node->ToElement();
    if (element != NULL) {
        string name = element->Name();
        if (name == "flames") {
            return serial::XMLElementClass::nodeDeserialize(node->FirstChild());
        }
    }
    return serial::XMLElementClass::nodeDeserialize(node);
}

}