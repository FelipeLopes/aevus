#include "xform.hpp"
#include "variation.hpp"

namespace core {

BaseXForm::BaseXForm(std::string tag): XMLElementClass(tag),
    color(*this, "color"),
    variationMap(*this, [](auto& names) {
        for (auto kv: Variation::variationNames.right) {
            names.insert(kv.first);
            auto paramNames = Variation::variationParamNames.find(kv.second)->second.paramNames;
            for (auto param: paramNames) {
                names.insert(kv.first + "_" + param);
            }
        }
    }),
    coefs(*this, "coefs"),
    post(*this, "post"),
    chaos(*this, "chaos"),
    colorSpeed(*this, {"color_speed", "symmetry"})
{
    variationMap.get()->variations[Variation::VariationID::LINEAR] =
        VariationData(1.0, {});
    colorSpeed.get()->colorSpeed = 0.5;
}

XFormCL BaseXForm::toXFormCL(int varBegin) const {
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

void BaseXForm::readVariationData(std::vector<VariationCL>& vars, std::vector<float>& params) {
    for (auto kv: variationMap.value().variations) {
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

XForm::XForm(): BaseXForm("xform"),
    weight(*this, "weight")
{
    weight.setValue(0.5);
}

FinalXForm::FinalXForm(): BaseXForm("finalxform") { }

}