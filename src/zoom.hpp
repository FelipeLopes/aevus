#pragma once

#include <cstdio>
#include <cmath>
#include <mutex>
#include <stdexcept>
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
        Builder(const DiverT& diver_, const ColorerT& colorer_): diver(diver_), colorer(colorer_) { }
        Builder& withShape(T r1_, T r2_, Complex<T> f_, bool flip_ = false) {
            initShape = true;
            r1 = r1_;
            r2 = r2_;
            f = f_;
            flip = flip_;
            return *this;
        }
        Builder& withScales(T iniLogscale_, T step_, int numSteps_, int precDigits_) {
            initScales = true;
            iniLogscale = iniLogscale_;
            step = step_;
            numSteps = numSteps_;
            precDigits = precDigits_;
            return *this;
        }
        Builder& withAspectRatio(T aspectRatio_) {
            initAspectRatio = true;
            aspectRatio = aspectRatio_;
            return *this;
        }
        Zoom build() {
            if (!initShape) {
                throw std::invalid_argument("Shape not initialized");
            }
            if (!initScales) {
                throw std::invalid_argument("Scales not initialized");
            }
            if (!initAspectRatio) {
                throw std::invalid_argument("Aspect ratio not initialized");
            }
            Shape<T> shape(r1, r2, f, flip);
            Scaler<T> scaler(iniLogscale, step, numSteps, precDigits);
            return Zoom(shape, diver, scaler, colorer, aspectRatio);
        }
    private:
        const DiverT& diver;
        const ColorerT& colorer;

        bool initShape = false;
        T r1, r2;
        Complex<T> f;
        bool flip;

        bool initScales = false;
        T iniLogscale, step;
        int numSteps, precDigits;

        bool initAspectRatio = false;
        T aspectRatio;
    };

    Flame getFlame(double logscale) const {
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
    Zoom(const Shape<T>& shape_, const DiverT& diver_,
        const Scaler<T>& scaler_, const ColorerT& colorer_, T ar_):
        ar(ar_), shape(shape_), diver(diver_), scaler(scaler_), colorer(colorer_) {

        static_assert(std::is_base_of<Diver<T>, DiverT>::value,
            "DiverT must implement Diver<T> interface");
        static_assert(std::is_base_of<Colorer, ColorerT>::value,
            "ColorerT must implement Colorer interface");

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

    T ar;
    const Shape<T>& shape;
    const DiverT& diver;
    const Scaler<T>& scaler;
    const ColorerT& colorer;
    std::vector<KeyGasket> keyGaskets;
    std::vector<int> diveIndices;
};
