#include <gmpxx.h>
#include <memory>
#include "key_gasket.hpp"
#include "mobius.hpp"

using std::vector;

KeyGasket::KeyGasket() {

}

KeyGasket::KeyGasket(std::vector<Mobius<double>> ifsTransforms_, double logscale_, int diveIndex_):
    logscale(logscale_), ifsTransforms(ifsTransforms_), diveIndex(diveIndex_) {

}

Flame KeyGasket::toFlame(Palette palette, double logscale) {
    Flame flame(palette);

    Mobius<double> s = Mobius<double>::scaling(exp(logscale));

    for (int i = 0; i<ifsTransforms.size(); i++) {
        auto t = ifsTransforms[i];
        double colorVal = (i == diveIndex) ? 1 : 0;
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
        flame.xforms[i].chaos[i] = 3;
        if (ifsTransforms.size() == 6) {
            flame.xforms[i+3].chaos[i+3] = 3;
        }
    }

    return flame;
}