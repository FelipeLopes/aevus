#include <gmpxx.h>
#include "key_gasket.h"
#include "mobius.h"

Flame KeyGasket::toFlame(double logscale) {
    Flame flame;

    Mobius<double> s = Mobius<double>::scaling(exp(logscale));

    for (auto t: ifsTransforms) {
        flame.xforms.push_back(t.conjugate(s).toXForm());
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