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
    colorSpeed(*this, {"color_speed", "symmetry"}) { }

XFormCL XForm::toXFormCL() const {
    XFormCL xf;

    auto pre = coefs.getValue();

    xf.a = pre.xx;
    xf.b = -pre.yx;
    xf.c = pre.ox;
    xf.d = -pre.xy;
    xf.e = pre.yy;
    xf.f = -pre.oy;

    auto pst = post.getValue();

    xf.pa = pst.xx;
    xf.pb = -pst.yx;
    xf.pc = pst.ox;
    xf.pd = -pst.xy;
    xf.pe = pst.yy;
    xf.pf = -pst.oy;

    int sz = 0;
    for (auto kv: variationMap.getValue().variations) {
        xf.varData[sz].id = kv.first;
        xf.varData[sz].weight = kv.second;
        sz++;
    }
    xf.varData[sz].id = Variation::NO_VARIATION;

    xf.color = color.getValue();
    xf.colorSpeed = colorSpeed.getValue().colorSpeed;

    return xf;
}

}