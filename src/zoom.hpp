#pragma once

#include <cstdio>
#include <cmath>
#include <mutex>
#include <type_traits>
#include <utility>
#include <memory>
#include <vector>

#include <boost/asio/thread_pool.hpp>
#include <tinyxml2.h>

#include "colorer.hpp"
#include "complex_type.hpp"
#include "diver.hpp"
#include "flame.hpp"
#include "key_gasket.hpp"
#include "mobius.hpp"
#include "scaler.hpp"
#include "sdf.hpp"
#include "searcher.hpp"
#include "shape.hpp"
#include "xform.hpp"

template <typename T, typename DiverT, typename ColorerT>
class Zoom {

using ColorParams = Colorer::ColorParams;

public:
    class Builder {
    public:
        Builder();
        Builder& withShape();
        Builder& withDiver();
        Builder& withScaler();
        Builder& withColorer();
        Builder& withAspectRatio();
        Zoom build();
    private:
        bool initShape = false;
        T r1, r2;
        Complex<T> f;
        bool flip;

        bool initDiver = false;
        DiverT diver;

        bool initScaler = false;
        T iniLogscale, step;
        int numSteps, prec;

        bool initColorer = false;
        ColorerT colorer;

        bool initAspectRatio = false;
        T aspectRatio;
    };

    Zoom(const Shape<T>& shape_, DiverT& diver_,
        const Scaler<T>& scaler_, const ColorerT& colorer_, T ar_):
        ar(ar_), shape(shape_), diver(diver_), scaler(scaler_), colorer(colorer_) {

        static_assert(std::is_base_of<Diver<T>, DiverT>::value,
            "DiverT must implement Diver<T> interface");

        Mobius<T> acc;
        int k = diver.chooseDive(acc);
        bool inverseDive = (k>=3);
        diveIndices.push_back(k);
        auto pts = shape.startingPoints(inverseDive);
        auto arr = shape.diveArray(inverseDive);
        acc = acc.compose(arr[k%3]);
        std::vector<Mobius<T>> zoomTransforms;
        zoomTransforms.push_back(acc);
        for (int i=0; i<diver.getDepth()-1; i++) {
            int k = diver.chooseDive(acc);
            diveIndices.push_back(k);
            acc = acc.compose(arr[k]);
            zoomTransforms.push_back(acc);
        }
        auto center = (acc.apply(pts[0])+acc.apply(pts[1])+acc.apply(pts[2]))/Complex<T>(3);

        Searcher<T> searcher(shape, scaler, center, inverseDive, zoomTransforms, keyGaskets, ar);

        searcher.start();
        searcher.block();
    }

    Flame getFlame(double logscale) {
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
        ColorParams params = colorer.color(keyGaskets[lb].numTransforms(), 0,
            logscale, keyGaskets[lb].logscale, keyGaskets[lb+1].logscale);
        return keyGaskets[lb].toFlame(params, logscale-keyGaskets[lb].logscale);
    }

private:
    T ar;
    const Shape<T>& shape;
    DiverT& diver;
    const Scaler<T>& scaler;
    const ColorerT& colorer;
    std::vector<KeyGasket> keyGaskets;
    std::vector<int> diveIndices;
};
