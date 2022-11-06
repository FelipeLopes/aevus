#include <bits/types/clock_t.h>
#include <cstddef>
#include <gmpxx.h>
#include <ctime>
#include <memory>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <boost/gil.hpp>
#include <tinyxml2.h>
#include "key_gasket.hpp"
#include "zoom.hpp"
#include "complex_type.hpp"
#include "mobius.hpp"
#include "palette.hpp"
#include "scaler.hpp"
#include "sdf.hpp"
#include "diver.hpp"

using boost::gil::rgb8_pixel_t;
using std::map;

void convertFlame(std::string source, std::string dest);

template<typename T>
class DiverImpl : public Diver<T> {
public:
    DiverImpl(): depth(200), rng(314159), dist2(0,1), dist3(0,2) {

    }
    int chooseDive(Mobius<T> acc) const {
        if (acc.a == Complex<T>(1) && acc.b == Complex<T>(0) &&
            acc.c == Complex<T>(0) && acc.d == Complex<T>(1)) {

            int k = dist2(rng);
            return k*3 + dist3(rng);
        }
        return dist3(rng);
    }
    int getDepth() const {
        return depth;
    }
private:
    int depth;
    mutable std::mt19937 rng;
    mutable std::uniform_int_distribution<std::mt19937::result_type> dist2, dist3;
};

class ColorerImpl: public Colorer {
public:
    ColorerImpl(): palette(WHITE, RED) {

    }
    ColorParams color(const map<double, KeyGasket>& keyGaskets,
        double logscale, int diveTransform) const {

        ColorParams params;
        params.palette = palette;
        auto it = std::prev(keyGaskets.lower_bound(logscale));
        int numTransforms = it->second.numTransforms();
        double iniKeyLogscale = it->first;
        double endKeyLogscale = std::next(it)->first;
        double f = (logscale-iniKeyLogscale)/(endKeyLogscale-iniKeyLogscale);
        double diveVal = std::min(1.0, 2*f);
        double nonDiveVal = std::max(0.0, 2*f-1);

        for (int i=0; i<numTransforms; i++) {
            double val = (i == diveTransform) ? diveVal : nonDiveVal;
            params.colorValues.push_back(val);
        }

        return params;
    }
private:
    Palette palette;
    static const boost::gil::rgb8_pixel_t RED, WHITE;
};

const rgb8_pixel_t ColorerImpl::RED = rgb8_pixel_t(255,0,0);
const rgb8_pixel_t ColorerImpl::WHITE = rgb8_pixel_t(255,255,255);

int main(int argc, char* argv[]) {
    try {
        ColorerImpl colorer;
        typedef Zoom<mpq_class, DiverImpl<mpq_class>, ColorerImpl> GasketZoom;
        const GasketZoom gz = GasketZoom::Builder()
            .withShape(mpq_class(6,11),mpq_class(3,7),Complex<mpq_class>(1))
            .withScales(mpq_class(-50,150), mpq_class(1,150), 22050)
            .withAspectRatio(mpq_class(16, 9))
            .build();

        for (int i=0; i<1; i++) {
            tinyxml2::XMLDocument xmlDoc;
            auto node = gz.getFlame(10+i*1./150).toXMLNode(xmlDoc);
            xmlDoc.InsertFirstChild(node);
            //std::ostringstream ss;
            //ss<<"/home/felipe/zoom/frame"<<std::setfill('0')<<std::setw(3)<<i<<".flame";
            //xmlDoc.SaveFile(ss.str().c_str());
            xmlDoc.SaveFile(stdout);
        }

    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}