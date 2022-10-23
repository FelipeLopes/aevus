#include <gmpxx.h>
#include "key_gasket.h"

Flame KeyGasket::toFlame() {
    Flame flame;
    for (auto t: ifsTransforms) {
        flame.xforms.push_back(t.toXForm());
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