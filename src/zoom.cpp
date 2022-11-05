#include <ctime>
#include <iostream>
#include <algorithm>
#include <memory>
#include <random>
#include <boost/asio/post.hpp>
#include <gmpxx.h>
#include <stdexcept>
#include <tuple>

#include "colorer.hpp"
#include "complex_type.hpp"
#include "mobius.hpp"
#include "zoom.hpp"
#include "sdf.hpp"

using std::shared_ptr;

template <typename T, typename DiverT, typename ColorerT>
Zoom<T, DiverT, ColorerT>::Zoom(const Shape<T>& shape_, DiverT& diver_,
    const Scaler<T>& scaler_, const ColorerT& colorer_, T ar_):
    ar(ar_), shape(shape_), diver(diver_), scaler(scaler_), colorer(colorer_) {

    Mobius<T> acc;
    int k = diver.chooseDive(acc);
    bool inverseDive = (k>=3);
    auto pts = shape.startingPoints(inverseDive);
    auto arr = shape.diveArray(inverseDive);
    acc = acc.compose(arr[k%3]);
    std::vector<Mobius<T>> zoomTransforms;
    zoomTransforms.push_back(acc);
    for (int i=0; i<diver.depth-1; i++) {
        int k = diver.chooseDive(acc);
        acc = acc.compose(arr[k]);
        zoomTransforms.push_back(acc);
    }
    auto center = (acc.apply(pts[0])+acc.apply(pts[1])+acc.apply(pts[2]))/Complex<T>(3);

    Searcher<T> searcher(shape, scaler, center, inverseDive, zoomTransforms, keyGaskets, ar);

    searcher.start();
    searcher.block();
}

template<typename T, typename DiverT, typename ColorerT>
Flame Zoom<T, DiverT, ColorerT>::getFlame(double logscale, shared_ptr<Palette> palette) {
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

template class Zoom<mpq_class, Diver<mpq_class>, Colorer>;