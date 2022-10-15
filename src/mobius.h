#pragma once

#include <gmpxx.h>
#include "complex_type.h"
#include "transform.h"

template <typename T>
class Mobius {
public:
    Mobius();
    Mobius(Complex<T> a, Complex<T> b, Complex<T> c, Complex<T> d);
    Complex<T> a, b, c, d;
    Complex<T> apply(Complex<T> z);
    Mobius<T> inverse();
    Mobius<T> compose(Mobius<T> n);
    Mobius<T> conjugate(Mobius<T> s);
    Mobius<T> flip();
    void normalize();
    XForm toXForm();
    static Mobius<T> fromPoints(Complex<T> p, Complex<T> q, Complex<T> r);
    static Mobius<T> fromPointsToPoints(Complex<T> p1, Complex<T> q1,
        Complex<T> r1, Complex<T> p2, Complex<T> q2, Complex<T> r2);
};

Mobius<mpq_class> su11(mpz_class m, mpz_class n, mpz_class p, mpz_class q);