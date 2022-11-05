#pragma once

#include "complex_type.hpp"
#include "mobius.hpp"

template <typename T>
class Shape {
public:
    Shape(T r1, T r2, Complex<T> f, bool flip = false);
    std::array<Complex<T>, 3> startingPoints(bool inverseDive) const;
    std::array<Mobius<T>, 3> diveArray(bool inverseDive) const;
    std::array<Mobius<double>, 6> doubleSidedTransforms(T scale, Complex<T> center) const ;
private:
    Complex<T> pa, pb, pc;
    Mobius<T> tr, rot;
};