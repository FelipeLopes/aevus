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

template <typename T>
class Zoom {
public:
    Zoom(std::shared_ptr<Shape<T>> shape, std::shared_ptr<Diver<T>> diver,
        std::shared_ptr<Scaler<T>> scaler, std::shared_ptr<Colorer> colorer, T ar);
    Flame getFlame(double logscale, std::shared_ptr<Palette> palette = nullptr);
private:
    std::vector<Mobius<T>> zoomTransforms;
    T ar;
    std::shared_ptr<Shape<T>> shape;
    std::shared_ptr<Diver<T>> diver;
    std::shared_ptr<Scaler<T>> scaler;
    std::shared_ptr<Searcher<T>> searcher;
    std::shared_ptr<Colorer> colorer;
    std::vector<KeyGasket> keyGaskets;
};
