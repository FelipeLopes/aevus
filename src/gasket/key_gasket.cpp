#include <cmath>
#include <gmpxx.h>
#include <memory>
#include "key_gasket.hpp"
#include "mobius.hpp"

namespace gasket {

using std::vector;

KeyGasket::KeyGasket(vector<Mobius<double>> ifsTransforms_, int level_):
    level(level_), ifsTransforms(ifsTransforms_) {

}

int KeyGasket::numTransforms() const {
    return ifsTransforms.size();
}

Flame KeyGasket::toFlame(int width, int height, ColorParams colorParams,
    double corr, double logscale) const
{
    Flame flame(width, height, colorParams.palette);

    Mobius<double> s = Mobius<double>::scaling(exp(logscale));

    for (int i = 0; i<ifsTransforms.size(); i++) {
        auto t = ifsTransforms[i];
        double colorVal = colorParams.colorValues[i];
        flame.xforms.push_back(t.conjugate(s).toXForm(colorVal));
    }

    for (int i = 0; i < 3; i++) {
        flame.xforms[i].chaos = {1, 1, 1};
        if (ifsTransforms.size() == 6) {
            flame.xforms[i].chaos.insert(flame.xforms[i].chaos.end(), {0, 0, 0});
            flame.xforms[i+3].chaos = {0, 0, 0, 1, 1, 1};
        }
    }

    for (int i=0; i<3; i++) {
        flame.xforms[i].chaos[i] = corr;
        if (ifsTransforms.size() == 6) {
            flame.xforms[i+3].chaos[i+3] = corr;
        }
    }

    return flame;
}

}