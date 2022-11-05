#pragma once

#include <gmpxx.h>
#include "complex_type.hpp"
#include "xform.hpp"

template <typename T>
class Mobius {
public:
    Mobius();
    Mobius(Complex<T> a, Complex<T> b, Complex<T> c, Complex<T> d);
    Complex<T> a, b, c, d;
    Complex<T> apply(Complex<T> z);
    Mobius<T> inverse() const;
    Mobius<T> compose(Mobius<T> n) const;
    Mobius<T> conjugate(Mobius<T> s) const;
    Mobius<T> flip();
    Mobius<double> toMobiusDouble();
    void normalize();
    XForm toXForm();
    static Mobius<T> scaling(Complex<T> a);
    static Mobius<T> translation(Complex<T> b);
    static Mobius<T> fromPoints(Complex<T> p, Complex<T> q, Complex<T> r);
    static Mobius<T> fromPointsToPoints(Complex<T> p1, Complex<T> q1,
        Complex<T> r1, Complex<T> p2, Complex<T> q2, Complex<T> r2);
};