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
#include "zoom.hpp"
#include "complex_type.hpp"
#include "mobius.hpp"
#include "palette.hpp"
#include "scaler.hpp"
#include "sdf.hpp"
#include "diver.hpp"

using boost::gil::rgb8_pixel_t;

void convertFlame(std::string source, std::string dest);

template<typename T>
class DiverImpl : public Diver<T> {
public:
    DiverImpl(int depth_, int seed): depth(depth_), rng(seed),
        dist2(0,1), dist3(0,2) {

    }
    int chooseDive(Mobius<T> acc) {
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
    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist2, dist3;
};

class ColorerImpl: public Colorer {
public:
    ColorerImpl(): palette(WHITE, RED) {

    }
    Colorer::ColorParams color(int numTransforms, int diveTransform, double logscale,
        double iniKeyLogscale, double endKeyLogscale) const {

        Colorer::ColorParams params;
        params.palette = palette;
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
        Shape<mpq_class> shape(mpq_class(6,11),mpq_class(3,7),
            Complex<mpq_class>(mpq_class(1,1),mpq_class(0,1)));
        DiverImpl<mpq_class> diver(200, 314159);
        ColorerImpl colorer;
        Scaler<mpq_class> scaler(mpq_class(-50,150), mpq_class(1,150), 22050, 10);
        typedef Zoom<mpq_class, DiverImpl<mpq_class>, ColorerImpl> GasketZoom;
        GasketZoom gz(shape, diver, scaler, colorer, mpq_class(16, 9));

        tinyxml2::XMLDocument xmlDoc;
        auto node = gz.getFlame(10).toXMLNode(xmlDoc);
        xmlDoc.InsertFirstChild(node);
        xmlDoc.SaveFile(stdout);

    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}