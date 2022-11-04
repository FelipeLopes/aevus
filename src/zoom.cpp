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
#include "zoom.hpp"
#include "sdf.hpp"

using std::shared_ptr;

template <typename T>
Zoom<T>::Zoom(shared_ptr<Shape<T>> shape_, shared_ptr<Diver<T>> diver_,
    shared_ptr<Scaler<T>> scaler_, shared_ptr<Colorer> colorer_, T ar_):
    ar(ar_), shape(shape_), diver(diver_), scaler(scaler_), colorer(colorer_) {

    Mobius<T> acc;
    int k = diver->chooseDive(acc);
    bool inverseDive = false;
    pa = shape->pa;
    pb = shape->pb;
    pc = shape->pc;
    dive = shape->tr;
    if (k >= 3) {
        inverseDive = true;
        std::swap(pb, pc);
        dive = shape->tr.inverse();
    }
    std::array<Mobius<T>,3> arr =
        {dive, dive.conjugate(shape->rot), dive.conjugate(shape->rot.inverse())};
    acc = acc.compose(arr[k%3]);
    zoomTransforms.push_back(acc);
    for (int i=0; i<diver->depth-1; i++) {
        int k = diver->chooseDive(acc);
        acc = acc.compose(arr[k]);
        zoomTransforms.push_back(acc);
    }
    center = (acc.apply(pa)+acc.apply(pb)+acc.apply(pc))/Complex<T>(3);

    searcher = std::make_shared<Searcher<T>>(shape, scaler, center, inverseDive,
        zoomTransforms, keyGaskets, ar);

    initZoom();
}

template <typename T>
void Zoom<T>::initZoom() {
    Sdf<T> sdf = Sdf<T>::fromPoints(pa, pb, pc);
    T iniScale = scaler->lookupExp(0);
    T iniHeight = 2/iniScale;
    T iniWidth = iniHeight*ar;
    if (!sdf.rectInside(center, iniWidth, iniHeight)) {
        KeyGasket g;
        g.logscale = toDouble(scaler->iniLogscale);

        auto s = Mobius<T>::scaling(scaler->lookupExp(0))
            .compose(Mobius<T>::translation(-center));

        g.ifsTransforms.push_back(shape->tr.conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(shape->tr.conjugate(shape->rot).conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(shape->tr.conjugate(shape->rot.inverse()).conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(shape->tr.inverse().conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(shape->tr.inverse().conjugate(shape->rot).conjugate(s).toMobiusDouble());
        g.ifsTransforms.push_back(shape->tr.inverse().conjugate(shape->rot.inverse()).conjugate(s).toMobiusDouble());

        keyGaskets.push_back(g);
    }
    searcher->start();
    searcher->block();
}

template<typename T>
Flame Zoom<T>::getFlame(double logscale, shared_ptr<Palette> palette) {
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

template class Zoom<mpq_class>;