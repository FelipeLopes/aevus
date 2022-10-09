#include <gmpxx.h>
#include "complex_type.h"

template <typename T>
Complex<T>::Complex(): real(Complex<T>::zero()), imag(Complex<T>::zero()) {

}

template <typename T>
Complex<T>::Complex(T a): real(a), imag(Complex<T>::zero()) {

}

template <typename T>
Complex<T>::Complex(T a, T b): real(a), imag(b) {

}

template <typename T>
T Complex<T>::norm() {
    return real*real + imag*imag;
}

template <typename T>
Complex<T> Complex<T>::conj() {
    return Complex<T>(real, -imag);
}

mpz_class integerSqrt(mpz_class s) {
    if (s < 0) {
        throw std::invalid_argument("Square root of negative integer.");
    }
    if (s == 0) {
        return 0;
    }
    mpz_class x0 = s / 2;
    mpz_class x1 = (x0 + s / x0) / 2;
    while (x1 < x0) {
        x0 = x1;
        x1 = (x0 + s / x0) / 2;
    }
    if (x0*x0 != s) {
        throw std::invalid_argument("Integer is not a perfect square.");
    }
    return x0;
}

mpq_class rationalSqrt(mpq_class x) {
    auto n = integerSqrt(x.get_num());
    auto d = integerSqrt(x.get_den());
    return mpq_class(n.get_str()+"/"+d.get_str());
}

Complex<mpq_class> complexSqrt(Complex<mpq_class> z) {
    auto x = z.real;
    auto y = z.imag;
    auto l = rationalSqrt(z.norm());
    mpq_class var = (l+x)/2;
    printf("%s\n", l.get_str().c_str());
    auto u = rationalSqrt((l+x)/2);
    auto v = rationalSqrt((l-x)/2);
    if (y < 0) {
        v = -v;
    }
    return Complex<mpq_class>(u,v);
}

template class Complex<mpq_class>;