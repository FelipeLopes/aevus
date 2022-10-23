#include <ctime>
#include <iostream>
#include <algorithm>
#include <random>
#include <boost/asio/post.hpp>
#include <gmpxx.h>
#include <tuple>

#include "complex_type.h"
#include "mobius.h"
#include "gasket.h"
#include "sdf.h"

template <typename T>
Gasket<T>::Gasket(T r1, T r2, Complex<T> f, bool flip): pool(4) {

    if (r2 > r1) {
        throw std::invalid_argument("First radius parameter should be greater than "
            "or equal to the second.");
    } else if (r1 + r2 > 1) {
        throw std::invalid_argument("Radii sum cannot be larger than 1.");
    } else if (r1 < 0 || r2 < 0) {
        throw std::invalid_argument("All radii should be positive.");
    }

    if (f.norm() != 1) {
        throw std::invalid_argument("Phase must have unit norm.");
    }

    T a = -1;
    T b = 1/r1;
    T c = 1/r2;
    T s1 = a + b + c;
    T s2 = a*b + b*c + c*a;

    T e = 2*squareRoot<T>(s2);
    T d = s1 - e;
    if (d <= 0) {
        d =  s1 + e;
    }
    if (d < c) {
        throw std::invalid_argument("Radii given are not the largest circles.");
    }

    T l1 = r1 + r2;
    T l2 = 1 - r1;
    T l3 = 1 - r2;
    T cosx = (l2*l2+l3*l3-l1*l1)/(2*l2*l3);
    T sinx = squareRoot<T>(1-cosx*cosx);

    Complex<T> ii(0,1);
    Complex<T> p1 = f;
    Complex<T> p2 = p1*(Complex<T>(cosx)+Complex<T>(sinx)*ii);

    Complex<T> v1 = (Complex<T>(1)-Complex<T>(r1))*p1;
    Complex<T> v2 = (Complex<T>(1)-Complex<T>(r2))*p2;
    Complex<T> v3 = (r2*v1+r1*v2)/Complex<T>(r1+r2);

    auto m = Mobius<T>::fromPointsToPoints(
        Complex<T>(1),
        Complex<T>(-1),
        Complex<T>(0),
        p1,p2,v3);

    if (flip) {
        m = m.flip();
    }

    tr = Mobius<T>(Complex<T>(1),Complex<T>(0),
        Complex<T>(2)*ii,Complex<T>(1)).conjugate(m);
    rot = Mobius<T>::fromPointsToPoints(
        Complex<T>(0), Complex<T>(1), Complex<T>(-1),
        Complex<T>(1), Complex<T>(-1), Complex<T>(0)).conjugate(m);

    tr.normalize();
    rot.normalize();

    center = Complex<T>(0);

    pa = m.apply(Complex<T>(-1));
    pb = m.apply(Complex<T>(1));
    pc = m.apply(Complex<T>(0));
}

template <typename T>
void Gasket<T>::setScales(T iniLogscale_, T step_, int numSteps_, T prec_) {
    iniLogscale = iniLogscale_;
    numSteps = numSteps_;
    step = step_;
    prec = prec_;
}

template <typename T>
Complex<T> Gasket<T>::selectZoomPoint(unsigned seed, int depth) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<std::mt19937::result_type> dist2(0,1);
    std::uniform_int_distribution<std::mt19937::result_type> dist3(0,2);
    dive = tr;
    if (dist2(rng) == 1) {
        std::swap(pb, pc);
        dive = tr.inverse();
    }
    std::array<Mobius<T>,3> arr =
        {dive, dive.conjugate(rot), dive.conjugate(rot.inverse())};
    Mobius<T> acc;
    zoomTransforms.clear();
    for (int i=0; i<depth; i++) {
        acc = acc.compose(arr[dist3(rng)]);
        zoomTransforms.push_back(acc);
    }
    center = (acc.apply(pa)+acc.apply(pb)+acc.apply(pc))/Complex<T>(3);
    return center;
}

template <typename T>
int Gasket<T>::searchScale(Sdf<T> shape, T ar) {
    int lb = 0;
    int ub = numSteps;
    while (ub - lb > 1) {
        int m = (lb + ub) / 2;
        T scale = lookupExp(m);
        T height = 2/scale;
        T width = height*ar;
        if (shape.rectInside(center, width, height)) {
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
    Sdf<T> shape = Sdf<T>::fromPoints(qa, qb, qc);
    int scaleVal = searchScale(shape, ar);
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
Flame Gasket<T>::getFlame(double logscale) {
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
    return keyGaskets[lb].toFlame(logscale-keyGaskets[lb].logscale);
}

template class Gasket<mpq_class>;