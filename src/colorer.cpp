#include <algorithm>
#include "colorer.hpp"
#include "palette.hpp"

using boost::gil::rgb8_pixel_t;
using std::max;
using std::min;

const rgb8_pixel_t Colorer::RED = rgb8_pixel_t(255,0,0);
const rgb8_pixel_t Colorer::WHITE = rgb8_pixel_t(255,255,255);

Colorer::Colorer(): palette(WHITE, RED) {

}

ColorParams Colorer::color(int numTransforms, int diveTransform, double logscale,
    double iniKeyLogscale, double endKeyLogscale) {

    ColorParams params;
    double f = (logscale-iniKeyLogscale)/(endKeyLogscale-iniKeyLogscale);
    double diveVal = min(1.0, 2*f);
    double nonDiveVal = max(0.0, 2*f-1);

    for (int i=0; i<numTransforms; i++) {
        double val = (i == diveTransform) ? diveVal : nonDiveVal;
        params.colorValues.push_back(val);
    }

    return params;
}