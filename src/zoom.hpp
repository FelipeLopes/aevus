#pragma once

#include <cstdio>
#include <cmath>
#include <mutex>
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
    Zoom(const Shape<T>& shape, DiverT& diver,
        const Scaler<T>& scaler, const ColorerT& colorer, T ar);
    Flame getFlame(double logscale);
private:
    T ar;
    const Shape<T>& shape;
    DiverT& diver;
    const Scaler<T>& scaler;
    const ColorerT& colorer;
    std::vector<KeyGasket> keyGaskets;
    std::vector<int> diveIndices;
};
