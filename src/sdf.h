#pragma once

#include <array>
#include "complex_type.h"

template <typename T>
class Sdf {
public:
    Sdf(T a, T b, T c, T d);
    bool inside(Complex<T> z);
    Sdf flip();
    static Sdf fromPoints(Complex<T> p, Complex<T> q, Complex<T> r);
private:
    T a, b, c, d;
    static T det33(std::array<std::array<T,3>,3> m);
};