#include <CL/cl.h>
#include <boost/gil.hpp>
#include <cstdlib>
#include <cinttypes>
#include <random>
#include <vector>
#include "gasket/zoom.hpp"
#include "render/cl_executable.hpp"
#include "render/iteration_state.hpp"
#include "render/opencl.hpp"
#include "render/xform_cl.hpp"
#include "render/xform_distribution.hpp"

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
        params.palette = (idx % 2 == 1) ? gasket::Palette(RED, WHITE) : gasket::Palette(WHITE, RED);
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
    const map<double, gasket::KeyGasket>* keyGasketsMap = nullptr;
    map<double, int> scaleIndex;
    static const boost::gil::rgb8_pixel_t RED, WHITE;
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
        auto flame = gz.getFlame(0, 10);
        auto node = flame.toXMLNode(xmlDoc);
        xmlDoc.InsertFirstChild(node);
        xmlDoc.SaveFile(stdout);

        auto context = render::OpenCL::getInstance().createContext(0,1);
        auto cmdQueue = context.createCommandQueue();
        auto stateBuf = context.createReadWriteBuffer(cmdQueue, 1024*sizeof(IterationState));
        auto xformBuf = context.createReadOnlyBuffer(cmdQueue, sizeof(render::XFormCL));
        auto outputBuf = context.createWriteOnlyBuffer(cmdQueue, 1024*2*sizeof(float));

        std::vector<IterationState> stateVec;
        std::mt19937_64 rng(314159);
        std::uniform_int_distribution<uint64_t> dist;
        std::uniform_real_distribution<float> floatDist(-1.0, 1.0);
        for (int i = 0; i < 1024; i++) {
            IterationState st;
            st.x = floatDist(rng);
            st.y = floatDist(rng);
            st.seed.value = dist(rng);
            stateVec.push_back(st);
        }

        stateBuf.write(stateVec);
        xformBuf.write(std::vector<render::XFormCL>{flame.xforms[0].toXFormCL()});

        auto kernel = context.createExecutable("iterate", "src/render/cl/iterate.cl");

        kernel.setArg(0, stateBuf);
        kernel.setArg(1, xformBuf);
        kernel.setArg(2, outputBuf);

        for (int i=0; i<100; i++) {
            kernel.run(cmdQueue, 1024, 64);
        }

        std::vector<IterationState> nStateVec;
        std::vector<float> ans;
        stateBuf.read(nStateVec);
        outputBuf.read(ans);

        for (int i=1014; i<1024; i++) {
            printf("(%f,%f)\n",ans[2*i],ans[2*i+1]);
        }

        render::XFormDistribution distrib;
        flame.readXFormDistribution(distrib);
        int ct[4];
        ct[0] = 0;
        ct[1] = 0;
        ct[2] = 0;
        ct[3] = 0;
        for (int i=0; i<16384; i++) {
            ct[distrib.data[2*16384+i]]++;
        }
        printf("%d\n",ct[0]);
        printf("%d\n",ct[1]);
        printf("%d\n",ct[2]);
        printf("%d\n",ct[3]);

    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}