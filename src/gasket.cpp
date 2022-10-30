#include <ctime>
#include <iostream>
#include <algorithm>
#include <memory>
#include <random>
#include <boost/asio/post.hpp>
#include <gmpxx.h>
#include <stdexcept>
#include <tuple>

#include "complex_type.hpp"
#include "mobius.hpp"
#include "gasket.hpp"
#include "sdf.hpp"

using std::invalid_argument;
using std::shared_ptr;

template <typename T>
Gasket<T>::Gasket(shared_ptr<Shape<T>> shape_, shared_ptr<Diver<T>> diver_):
    pool(4), shape(shape_), diver(diver_) {

    auto m = shape->conjugacyTransform;

    tr = Mobius<T>(Complex<T>(1),Complex<T>(0),
        Complex<T>(0,2),Complex<T>(1)).conjugate(m);
    rot = Mobius<T>::fromPointsToPoints(
        Complex<T>(0), Complex<T>(1), Complex<T>(-1),
        Complex<T>(1), Complex<T>(-1), Complex<T>(0)).conjugate(m);

    pa = m.apply(Complex<T>(-1));
    pb = m.apply(Complex<T>(1));
    pc = m.apply(Complex<T>(0));

    selectZoomPoint();
}

template <typename T>
void Gasket<T>::setScales(T iniLogscale_, T step_, int numSteps_, T prec_) {
    iniLogscale = iniLogscale_;
    numSteps = numSteps_;
    step = step_;
    prec = prec_;
}

template <typename T>
void Gasket<T>::selectZoomPoint() {
    Mobius<T> acc;
    int k = diver->chooseDive(acc);
    dive = tr;
    if (k >= 3) {
        std::swap(pb, pc);
        dive = tr.inverse();
    }
    std::array<Mobius<T>,3> arr =
        {dive, dive.conjugate(rot), dive.conjugate(rot.inverse())};
    acc = acc.compose(arr[k%3]);
    zoomTransforms.push_back(acc);
    for (int i=0; i<diver->depth-1; i++) {
        int k = diver->chooseDive(acc);
        acc = acc.compose(arr[k]);
        zoomTransforms.push_back(acc);
    }
    center = (acc.apply(pa)+acc.apply(pb)+acc.apply(pc))/Complex<T>(3);
}

template <typename T>
int Gasket<T>::searchScale(Sdf<T> sdf, T ar) {
    int lb = 0;
    int ub = numSteps;
    while (ub - lb > 1) {
        int m = (lb + ub) / 2;
        T scale = lookupExp(m);
        T height = 2/scale;
        T width = height*ar;
        if (sdf.rectInside(center, width, height)) {
            ub = m;
        } else {
            lb = m;
        }
    }
    return ub;
}

template <typename T>
T Gasket<T>::lookupExp(int n) {
    T ans = base;
    while (n > 0) {
        int bits = (n & -n);
        ans = ans * lookup[__builtin_ctz(bits)];
        n -= bits;
    }
    return ans;
}

template<typename T>
void Gasket<T>::task(int i) {
    Mobius<T> acc = zoomTransforms[i];
    auto qa = acc.apply(pa);
    auto qb = acc.apply(pb);
    auto qc = acc.apply(pc);
    Sdf<T> sdf = Sdf<T>::fromPoints(qa, qb, qc);
    int scaleVal = searchScale(sdf, ar);
    KeyGasket g;
    T logscale = iniLogscale + scaleVal*step;
    g.logscale = toDouble(logscale);
    auto s = Mobius<T>::scaling(lookupExp(scaleVal))
        .compose(Mobius<T>::translation(-center))
        .compose(acc);

    g.ifsTransforms.push_back(dive.conjugate(s).toMobiusDouble());
    g.ifsTransforms.push_back(dive.conjugate(rot).conjugate(s).toMobiusDouble());
    g.ifsTransforms.push_back(dive.conjugate(rot.inverse()).conjugate(s).toMobiusDouble());

    initLock.lock();
    if (scaleVal < numSteps) {
        if (keyGaskets.size() < i+2) {
            keyGaskets.resize(i+2);
        }
        keyGaskets[i+1] = g;
    } else {
        foundEnd = true;
    }
    if (!foundEnd) {
        lastPickedUp++;
        boost::asio::post(pool, [=] {
            task(lastPickedUp);
        });
    }
    initLock.unlock();
}

template <typename T>
void Gasket<T>::initZoom(T ar_) {
    ar = ar_;
    base = exp<T>(iniLogscale, prec);
    lookup.resize(32-__builtin_clz(numSteps));
    for (int i=0; i<lookup.size(); i++) {
        lookup[i] = exp<T>((1<<i)*step, prec);
    }
    Sdf<T> shape = Sdf<T>::fromPoints(pa, pb, pc);
    T iniScale = lookupExp(0);
    T iniHeight = 2/iniScale;
    T iniWidth = iniHeight*ar;
    if (!shape.rectInside(center, iniWidth, iniHeight)) {
        KeyGasket g;
        g.logscale = toDouble(iniLogscale);

        auto s = Mobius<T>::scaling(lookupExp(0))
            .compose(Mobius<T>::translation(-center));

        g.ifsTransforms.push_back(tr.conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(tr.conjugate(rot).conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(tr.conjugate(rot.inverse()).conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(tr.inverse().conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(tr.inverse().conjugate(rot).conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(tr.inverse().conjugate(rot.inverse()).conjugate(s).toMobiusDouble());

        keyGaskets.push_back(g);
    }
    lastPickedUp = 3;
    foundEnd = false;
    for (int i=0; i<4; i++) {
        boost::asio::post(pool, [=] {
            task(i);
        });
    }
    pool.join();
}

template<typename T>
Flame Gasket<T>::getFlame(double logscale, shared_ptr<Palette> palette) {
    int lb = 0;
    int ub = keyGaskets.size();
    while (ub - lb > 1) {
        int m = (lb + ub) / 2;
        if (keyGaskets[m].logscale < logscale) {
            lb = m;
        } else {
            ub = m;
        }
    }
    return keyGaskets[lb].toFlame(logscale-keyGaskets[lb].logscale, palette);
}

template class Gasket<mpq_class>;