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

XForm::XForm(): BaseXForm("xform"),
    weight(*this, "weight")
{
    weight.setValue(0.5);
}

FinalXForm::FinalXForm(): BaseXForm("finalxform") { }

}