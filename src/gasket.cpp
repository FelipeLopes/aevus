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
Gasket<T>::Gasket(shared_ptr<Shape<T>> shape_, shared_ptr<Diver<T>> diver_,
    shared_ptr<Scaler<T>> scaler_):
    shape(shape_), diver(diver_), scaler(scaler_) {

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

    std::array<Mobius<T>, 3> arr =
        {dive, dive.conjugate(rot), dive.conjugate(rot.inverse())};

    searcher = std::make_shared<Searcher<T>>(scaler, pa, pb, pc, center,
        arr, zoomTransforms, keyGaskets, ar);
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
void Gasket<T>::initZoom(T ar_) {
    ar = ar_;
    Sdf<T> shape = Sdf<T>::fromPoints(pa, pb, pc);
    T iniScale = scaler->lookupExp(0);
    T iniHeight = 2/iniScale;
    T iniWidth = iniHeight*ar;
    if (!shape.rectInside(center, iniWidth, iniHeight)) {
        KeyGasket g;
        g.logscale = toDouble(scaler->iniLogscale);

        auto s = Mobius<T>::scaling(scaler->lookupExp(0))
            .compose(Mobius<T>::translation(-center));

        g.ifsTransforms.push_back(tr.conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(tr.conjugate(rot).conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(tr.conjugate(rot.inverse()).conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(tr.inverse().conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(tr.inverse().conjugate(rot).conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(tr.inverse().conjugate(rot.inverse()).conjugate(s).toMobiusDouble());

        keyGaskets.push_back(g);
    }
    searcher->start();
    searcher->block();
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