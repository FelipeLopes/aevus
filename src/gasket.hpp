#pragma once

#include <cstdio>
#include <cmath>
#include <mutex>
#include <utility>
#include <memory>
#include <vector>

#include <boost/asio/thread_pool.hpp>
#include <tinyxml2.h>

#include "complex_type.hpp"
#include "diver.hpp"
#include "flame.hpp"
#include "key_gasket.hpp"
#include "mobius.hpp"
#include "scaler.hpp"
#include "sdf.hpp"
#include "shape.hpp"
#include "xform.hpp"

template <typename T>
class Gasket {
public:
    Gasket(std::shared_ptr<Shape<T>> shape, std::shared_ptr<Diver<T>> diver,
        std::shared_ptr<Scaler<T>> scaler);
    void setScales(T iniLogscale, T step, int numSteps, T prec);
    void initZoom(T ar);
    Flame getFlame(double logscale, std::shared_ptr<Palette> palette = nullptr);
private:
    void task(int i);
    void selectZoomPoint();
    int searchScale(Sdf<T> sdf, T ar);
    std::vector<Mobius<T>> zoomTransforms;
    std::vector<T> lookup;
    T base, ar;
    T lookupExp(int n);
    boost::asio::thread_pool pool;
    std::shared_ptr<Shape<T>> shape;
    std::shared_ptr<Diver<T>> diver;
    std::shared_ptr<Scaler<T>> scaler;
    std::mutex initLock;
    int lastPickedUp;
    bool foundEnd;
    std::vector<KeyGasket> keyGaskets;
    Complex<T> pa, pb, pc;
    Mobius<T> tr, rot, dive;
    bool doubleSided = true;
    Complex<T> center;
    T iniLogscale, step, prec;
    int numSteps;
};
