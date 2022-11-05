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
    Zoom(const Shape<T>& shape, DiverT& diver,
        const Scaler<T>& scaler, const ColorerT& colorer, T ar);
    Flame getFlame(double logscale, std::shared_ptr<Palette> palette = nullptr);
private:
    T ar;
    const Shape<T>& shape;
    DiverT& diver;
    const Scaler<T>& scaler;
    const ColorerT& colorer;
    std::vector<KeyGasket> keyGaskets;
    std::vector<int> diveIndices;
};
