#pragma once

#include <cstdio>
#include <cmath>
#include <mutex>
#include <utility>
#include <memory>
#include <vector>

#include <boost/asio/thread_pool.hpp>
#include <tinyxml2.h>

#include "complex_type.h"
#include "flame.h"
#include "key_gasket.h"
#include "mobius.h"
#include "sdf.h"
#include "xform.h"

template <typename T>
class Gasket {
public:
    Gasket(T r1, T r2, Complex<T> f, bool flip = false);
    void setScales(T iniLogscale, T step, int numSteps, T prec);
    Complex<T> selectZoomPoint(unsigned seed, int depth);
    void initZoom(T ar);
private:
    std::array<Mobius<T>, 3> mobiusArray();
    void task(int i);
    int searchScale(Sdf<T> shape, T ar);
    std::vector<int> vals;
    std::vector<Mobius<T>> zoomTransforms;
    std::vector<T> lookup;
    T base, ar;
    T lookupExp(int n);
    boost::asio::thread_pool pool;
    std::mutex initLock;
    int lastPickedUp;
    bool foundEnd;
    std::vector<KeyGasket<T>> keyGaskets;
    Complex<T> pa, pb, pc;
    Mobius<T> tr, rot, dive;
    bool doubleSided = true;
    Complex<T> center;
    T iniLogscale, step, prec;
    int numSteps;
};
