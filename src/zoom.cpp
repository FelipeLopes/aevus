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
    bool inverseDive = (k>=3);
    auto pts = shape->startingPoints(inverseDive);
    auto arr = shape->diveArray(inverseDive);
    acc = acc.compose(arr[k%3]);
    zoomTransforms.push_back(acc);
    for (int i=0; i<diver->depth-1; i++) {
        int k = diver->chooseDive(acc);
        acc = acc.compose(arr[k]);
        zoomTransforms.push_back(acc);
    }
    auto center = (acc.apply(pts[0])+acc.apply(pts[1])+acc.apply(pts[2]))/Complex<T>(3);

    searcher = std::make_shared<Searcher<T>>(shape, scaler, center, inverseDive,
        zoomTransforms, keyGaskets, ar);

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