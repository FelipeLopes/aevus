#pragma once

#include "complex_type.hpp"
#include "mobius.hpp"

template <typename T>
class Shape {
public:
    Shape(T r1, T r2, Complex<T> f, bool flip = false);
    std::array<Complex<T>, 3> startingPoints(bool inverseDive);
    std::array<Mobius<T>, 3> diveArray(bool inverseDive);
    Complex<T> pa, pb, pc;
    Mobius<T> tr, rot;
};