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

    selectZoomPoint();

    std::array<Mobius<T>, 3> arr =
        {dive, dive.conjugate(shape->rot), dive.conjugate(shape->rot.inverse())};

    searcher = std::make_shared<Searcher<T>>(scaler, shape->pa, shape->pb, shape->pc, 
        shape->center, arr, zoomTransforms, keyGaskets, ar);

    initZoom();
}

template <typename T>
void Zoom<T>::selectZoomPoint() {
    Mobius<T> acc;
    int k = diver->chooseDive(acc);
    dive = shape->tr;
    if (k >= 3) {
        std::swap(shape->pb, shape->pc);
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
    shape->center = (acc.apply(shape->pa)+acc.apply(shape->pb)+
        acc.apply(shape->pc))/Complex<T>(3);
}

template <typename T>
void Zoom<T>::initZoom() {
    Sdf<T> sdf = Sdf<T>::fromPoints(shape->pa, shape->pb, shape->pc);
    T iniScale = scaler->lookupExp(0);
    T iniHeight = 2/iniScale;
    T iniWidth = iniHeight*ar;
    if (!sdf.rectInside(shape->center, iniWidth, iniHeight)) {
        KeyGasket g;
        g.logscale = toDouble(scaler->iniLogscale);

        auto s = Mobius<T>::scaling(scaler->lookupExp(0))
            .compose(Mobius<T>::translation(-shape->center));

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