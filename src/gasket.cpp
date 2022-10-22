#include <ctime>
#include <iostream>
#include <algorithm>
#include <random>
#include <gmpxx.h>
#include <tuple>

#include "complex_type.h"
#include "mobius.h"
#include "gasket.h"
#include "sdf.h"

template <typename T>
Gasket<T>::Gasket(T r1, T r2, Complex<T> f, bool flip) {

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
    scale = 1;

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
    scale = exp<T>(iniLogscale + step*numSteps, prec);
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
    auto arr = mobiusArray();
    Mobius<T> acc;
    vals.clear();
    for (int i=0; i<depth; i++) {
        int k = dist3(rng);
        vals.push_back(k);
        acc = acc.compose(arr[k]);
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
        T scale = exp<T>(iniLogscale + m*step, prec);
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
void Gasket<T>::adapt(T ar) {
    auto arr = mobiusArray();
    Sdf<T> shape = Sdf<T>::fromPoints(pa, pb, pc);
    T height = 2/scale;
    T width = height*ar;
    if (shape.rectInside(center, width, height)) {
        doubleSided = false;
    }
    Sdf<T> shape_ = Sdf<T>::fromPoints(pa, pb, pc);
    T iniScale = exp<T>(iniLogscale, prec);
    T iniHeight = 2/iniScale;
    T iniWidth = iniHeight*ar;
    if (!shape.rectInside(center, iniWidth, iniHeight)) {
        KeyGasket<T> g;
        g.logscale = iniLogscale;

        auto s = Mobius<T>::scaling(Complex<T>(scale))
            .compose(Mobius<T>::translation(-center));

        g.ifsTransforms.push_back(tr.conjugate(s));
        g.ifsTransforms.push_back(tr.conjugate(rot).conjugate(s));
        g.ifsTransforms.push_back(tr.conjugate(rot.inverse()).conjugate(s));
        g.ifsTransforms.push_back(tr.inverse().conjugate(s));
        g.ifsTransforms.push_back(tr.inverse().conjugate(rot).conjugate(s));
        g.ifsTransforms.push_back(tr.inverse().conjugate(rot.inverse()).conjugate(s));

        keyGaskets.push_back(g);
    }
    Mobius<T> acc_;
    int i = 0;
    while (true) {
        auto aux = acc_.compose(arr[vals[i]]);
        auto qa = aux.apply(pa);
        auto qb = aux.apply(pb);
        auto qc = aux.apply(pc);
        shape = Sdf<T>::fromPoints(qa, qb, qc);
        int scaleVal = searchScale(shape, ar);
        if (scaleVal == numSteps) {
            break;
        }
        KeyGasket<T> g;
        g.logscale = iniLogscale + scaleVal*step;
        auto s = Mobius<T>::scaling(Complex<T>(scale))
            .compose(Mobius<T>::translation(-center))
            .compose(acc_);

        g.ifsTransforms.push_back(dive.conjugate(s));
        g.ifsTransforms.push_back(dive.conjugate(rot).conjugate(s));
        g.ifsTransforms.push_back(dive.conjugate(rot.inverse()).conjugate(s));

        keyGaskets.push_back(g);

        acc_ = aux;
        i++;
    }
    for (auto kg: keyGaskets) {
        std::cout<<kg.logscale<<std::endl;
    }
    Mobius<T> acc;
    if (!doubleSided) {
        int i = 0;
        while (true) {
            auto aux = acc.compose(arr[vals[i]]);
            auto qa = aux.apply(pa);
            auto qb = aux.apply(pb);
            auto qc = aux.apply(pc);
            shape = Sdf<T>::fromPoints(qa, qb, qc);
            if (!shape.rectInside(center, width, height)) {
                break;
            }
            acc = aux;
            i++;
        }
    }
    auto s = Mobius<T>::scaling(Complex<T>(scale))
        .compose(Mobius<T>::translation(-center))
        .compose(acc);
    if (doubleSided) {
        ifsTransforms.push_back(tr.conjugate(s));
        ifsTransforms.push_back(tr.conjugate(rot).conjugate(s));
        ifsTransforms.push_back(tr.conjugate(rot.inverse()).conjugate(s));
        ifsTransforms.push_back(tr.inverse().conjugate(s));
        ifsTransforms.push_back(tr.inverse().conjugate(rot).conjugate(s));
        ifsTransforms.push_back(tr.inverse().conjugate(rot.inverse()).conjugate(s));
    } else {
        ifsTransforms.push_back(dive.conjugate(s));
        ifsTransforms.push_back(dive.conjugate(rot).conjugate(s));
        ifsTransforms.push_back(dive.conjugate(rot.inverse()).conjugate(s));
    }
}

template <typename T>
std::array<Mobius<T>,3> Gasket<T>::mobiusArray() {
    return {dive, dive.conjugate(rot), dive.conjugate(rot.inverse())};
}

template<typename T>
Flame Gasket<T>::toFlame() {
    Flame flame;
    auto s = Mobius<T>::scaling(Complex<T>(scale)).compose(Mobius<T>::translation(-center));
    for (auto t: ifsTransforms) {
        flame.xforms.push_back(t.toXForm());
    }

    for (int i = 0; i < 3; i++) {
        flame.xforms[i].chaos = {1, 1, 1};
        if (doubleSided) {
            flame.xforms[i].chaos.insert(flame.xforms[i].chaos.end(), {0, 0, 0});
            flame.xforms[i+3].chaos = {0, 0, 0, 1, 1, 1};
        }
    }

    for (int i=0; i<3; i++) {
        flame.xforms[i].chaos[i] = 3;
        if (doubleSided) {
            flame.xforms[i+3].chaos[i+3] = 3;
        }
    }

    return flame;
}

template class Gasket<mpq_class>;