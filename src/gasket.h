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
    void setCenter(Complex<T> center);
    void setScale(double scale);
    Flame toFlame();
private:
    Mobius<T> tr, rot;
    Complex<T> center;
    double scale;
};
