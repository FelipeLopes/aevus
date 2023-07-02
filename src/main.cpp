#include <CL/cl.h>
#include <boost/gil.hpp>
#include <cfloat>
#include <cstdlib>
#include <cinttypes>
#include <random>
#include <vector>
#include "gasket/zoom.hpp"
#include "render/cl_buffer.hpp"
#include "render/cl_executable.hpp"
#include "render/color_cl.hpp"
#include "render/iteration_state.hpp"
#include "render/iterator.hpp"
#include "render/opencl.hpp"
#include "render/palette.hpp"
#include "render/variation.hpp"
#include "render/xform.hpp"
#include "render/xform_cl.hpp"

using boost::gil::rgb8_pixel_t;
using std::map;

template<typename T>
class DiverImpl : public gasket::Diver<T> {
public:
    DiverImpl(int depth_, int seed): depth(depth_), rng(seed), dist2(0,1), dist3(0,2) {

    }
    int chooseDive(gasket::Mobius<T> acc) const {
        if (acc.a == gasket::Complex<T>(1) && acc.b == gasket::Complex<T>(0) &&
            acc.c == gasket::Complex<T>(0) && acc.d == gasket::Complex<T>(1)) {

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

class ColorerImpl: public gasket::Colorer {
public:
    ColorerImpl() { }
    void keyGaskets(const map<double, gasket::KeyGasket>& keyGaskets) {
        keyGasketsMap = &keyGaskets;
        int idx = 0;
        for (auto kg: keyGaskets) {
            scaleIndex[kg.first] = idx++;
        }
    }
    gasket::ColorParams color(double logscale, int diveTransform) const {
        gasket::ColorParams params;
        auto it = std::prev(keyGasketsMap->lower_bound(logscale));
        int idx = scaleIndex.find(it->first)->second;
        params.palette = (idx % 2 == 1) ? render::Palette(RED, WHITE) : render::Palette(WHITE, RED);
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
    static const boost::gil::rgb8_pixel_t RED, WHITE;
private:
    const map<double, gasket::KeyGasket>* keyGasketsMap = nullptr;
    map<double, int> scaleIndex;
};

const rgb8_pixel_t ColorerImpl::RED = rgb8_pixel_t(255,0,0);
const rgb8_pixel_t ColorerImpl::WHITE = rgb8_pixel_t(255,255,255);

int main(int argc, char* argv[]) {
    try {
        DiverImpl<mpq_class> diver(200, 314159);
        ColorerImpl colorer;
        typedef gasket::Zoom<mpq_class, DiverImpl<mpq_class>, ColorerImpl> GasketZoom;
        const GasketZoom gz = GasketZoom::Builder()
            .withShape(mpq_class(6,11),mpq_class(3,7),gasket::Complex<mpq_class>(1))
            .withScales(mpq_class(-50,150), mpq_class(1,150), 22050)
            .withImageSize(480, 270)
            .build(diver, colorer);
        /*
        for (int i=0; i<900; i++) {
            tinyxml2::XMLDocument xmlDoc;
            auto node = gz.getFlame(20+i*1./150).toXMLNode(xmlDoc);
            xmlDoc.InsertFirstChild(node);
            std::ostringstream ss;
            ss<<"/home/felipe/zoom/frame"<<std::setfill('0')<<std::setw(3)<<i<<".flame";
            xmlDoc.SaveFile(ss.str().c_str());
        }*/

        tinyxml2::XMLDocument xmlDoc;
        auto flame = gz.getFlame(3, 10);
        /*auto node = flame.toXMLNode(xmlDoc);
        xmlDoc.InsertFirstChild(node);
        xmlDoc.SaveFile(stdout);*/

        auto squareFlame = render::Flame(0, 0, 400, 400, render::Palette(ColorerImpl::WHITE));
        squareFlame.scale = 400;
        render::XForm xform;
        xform.variations[render::Variation::VariationID::SQUARE] = 1;
        xform.chaos.resize(1);
        xform.chaos[0]=1;
        squareFlame.xforms.push_back(xform);

        auto node = squareFlame.toXMLNode(xmlDoc);
        xmlDoc.InsertFirstChild(node);
        xmlDoc.SaveFile(stdout);

        auto context = render::OpenCL::getInstance().createQueuedContext(0,1);

        render::Iterator iterator(context, squareFlame, 200, 20);

        //iterator.writeImage("gasket.pam");

    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}