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

template <>
mpz_class squareRoot<mpz_class>(mpz_class s) {
    if (s < 0) {
        throw std::invalid_argument("Square root of negative integer.");
    }
    if (s == 0 || s == 1) {
        return s;
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

template <>
mpq_class squareRoot<mpq_class>(mpq_class x) {
    x.canonicalize();
    auto n = squareRoot<mpz_class>(x.get_num());
    auto d = squareRoot<mpz_class>(x.get_den());
    return mpq_class(n.get_str()+"/"+d.get_str());
}

template<>
Complex<mpq_class> squareRoot<Complex<mpq_class>>(Complex<mpq_class> z) {
    auto x = z.real;
    auto y = z.imag;
    auto l = squareRoot<mpq_class>(z.norm());
    auto u = squareRoot<mpq_class>((l+x)/2);
    auto v = squareRoot<mpq_class>((l-x)/2);
    if (y < 0) {
        v = -v;
    }
    return Complex<mpq_class>(u,v);
}

template class Complex<mpq_class>;