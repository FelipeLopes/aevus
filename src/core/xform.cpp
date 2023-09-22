#include "xform.hpp"

namespace core {

XForm::XForm(): XMLElementClass("xform"),
    weight(*this, "weight"),
    color(*this, "color"),
    variationMap(*this, [](auto& names) {
        for (auto kv: Variation::variationNames.right) {
            names.insert(kv.first);
        }
    }),
    coefs(*this, "coefs"),
    post(*this, "post"),
    chaos(*this, "chaos"),
    colorSpeed(*this, {"color_speed", "symmetry"})
{
    weight.setValue(0.5);
    variationMap.get()->variations[Variation::VariationID::LINEAR] =
        VariationData(1.0, {});
    colorSpeed.get()->colorSpeed = 0.5;
}

XFormCL XForm::toXFormCL(int varBegin) const {
    XFormCL xf;

    auto pre = coefs.value();

    xf.a = pre.xx;
    xf.b = -pre.yx;
    xf.c = pre.ox;
    xf.d = -pre.xy;
    xf.e = pre.yy;
    xf.f = -pre.oy;

    auto pst = post.value();

    xf.pa = pst.xx;
    xf.pb = -pst.yx;
    xf.pc = pst.ox;
    xf.pd = -pst.xy;
    xf.pe = pst.yy;
    xf.pf = -pst.oy;

    xf.varBegin = varBegin;
    xf.varEnd = varBegin + variationMap.value().variations.size();

    xf.color = color.value();
    xf.colorSpeed = colorSpeed.value().colorSpeed;

    return xf;
}

void XForm::readVariationCLArray(std::vector<VariationCL>& vars) {
    for (auto kv: variationMap.value().variations) {
        VariationCL varCL;
        varCL.id = kv.first;
        varCL.weight = kv.second.weight;
        varCL.paramBegin = 0;
        vars.push_back(varCL);
    }
}

}