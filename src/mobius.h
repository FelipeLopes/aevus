#include <gmpxx.h>
#include "complex_type.h"

template <typename T>
class Mobius {
public:
    Mobius(Complex<T> a, Complex<T> b, Complex<T> c, Complex<T> d);
    Complex<T> a, b, c, d;
    Mobius<T> inverse();
    Mobius<T> compose(Mobius<T> n);
    Mobius<T> conjugate(Mobius<T> s);
    static Mobius<T> fromPoints(Complex<T> p, Complex<T> q, Complex<T> r);
    static Mobius<T> fromPointsToPoints(Complex<T> p1, Complex<T> q1,
        Complex<T> r1, Complex<T> p2, Complex<T> q2, Complex<T> r2);
};

template <typename T>
Mobius<T> Mobius<T>::fromPoints(Complex<T> p, Complex<T> q, Complex<T> r) {
    return Mobius<T>(q-r,-p*(q-r),(q-p),-r*(q-p));
}

template <typename T>
Mobius<T> Mobius<T>::fromPointsToPoints(Complex<T> p1, Complex<T> q1,
    Complex<T> r1, Complex<T> p2, Complex<T> q2, Complex<T> r2) {

    return fromPoints(p2, q2, r2).inverse().compose(fromPoints(p1, q1, r1));
}

Mobius<mpq_class> su11(mpz_class m, mpz_class n, mpz_class p, mpz_class q);