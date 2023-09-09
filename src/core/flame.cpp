#include "flame.hpp"
#include <memory>
#include <random>

using std::string;
using std::vector;

namespace core {

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
    xforms(*this, "xform"),
    palette(*this)
{
    center.get()->x = 0;
    center.get()->y = 0;
    scale.setValue(100);
    size.get()->width = 732;
    size.get()->height = 640;
    xforms.append(std::make_shared<XForm>());
}

FlameCL Flame::getFlameCL() const {
    FlameCL flameCL;
    flameCL.cx = center.value().x;
    flameCL.cy = center.value().y;
    flameCL.scale = scale.value();
    flameCL.width = size.value().width;
    flameCL.height = size.value().height;
    return flameCL;
}

void Flame::readInitialStateArray(std::vector<IterationState> &arr, int size) const {
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

void Flame::readXFormCLArray(std::vector<XFormCL>& arr) const {
    arr.resize(xforms.size());
    for (int i=0; i<arr.size(); i++) {
        arr[i] = xforms.get(i)->toXFormCL();
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

}