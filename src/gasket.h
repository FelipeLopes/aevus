#pragma once

#include <cstdio>
#include <cmath>
#include <utility>
#include <memory>
#include <vector>

#include <tinyxml2.h>

#include "complex_type.h"
#include "flame.h"
#include "mobius.h"
#include "xform.h"

template <typename T>
class Gasket {
public:
    Gasket(T r1, T r2, Complex<T> f, bool flip = false);
    void setScale(T scale);
    Complex<T> selectZoomPoint(unsigned seed, int depth);
    Flame toFlame();
    void adapt(T ar);
private:
    std::array<Mobius<T>, 3> mobiusArray();
    std::vector<int> vals;
    std::vector<Mobius<T>> ifsTransforms;
    Complex<T> pa, pb, pc;
    Mobius<T> tr, rot, dive;
    bool doubleSided = true;
    Complex<T> center;
    T scale;
};
