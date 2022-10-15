#pragma once

#include <complex>
#include <gmpxx.h>

typedef std::complex<double> cx;

template <typename T>
class Complex {
public:
    Complex();
    Complex(T a);
    Complex(T a, T b);
    T real, imag;
    T norm();
    Complex<T> conj();
    cx toCxDouble();
    void print();
};

template<typename T>
bool operator==(Complex<T> a, Complex<T> b) {
    return a.real == b.real && a.imag == b.imag;
}

template<typename T>
bool operator!=(Complex<T> a, Complex<T> b) {
    return a.real != b.real || a.imag != b.imag;
}

template<typename T>
Complex<T> operator+(Complex<T> a, Complex<T> b) {
    return Complex<T>(a.real+b.real, a.imag+b.imag);
}

template<typename T>
Complex<T> operator-(Complex<T> a) {
    return Complex<T>(-a.real, -a.imag);
}

template<typename T>
Complex<T> operator-(Complex<T> a, Complex<T> b) {
    return Complex<T>(a.real - b.real, a.imag - b.imag);
}

template<typename T>
Complex<T> operator*(T a, Complex<T> b) {
    return Complex<T>(a*b.real,a*b.imag);
}

template<typename T>
Complex<T> operator*(Complex<T> a, Complex<T> b) {
    return Complex<T>(a.real*b.real-a.imag*b.imag, a.real*b.imag+a.imag*b.real);
}

template<typename T>
Complex<T> operator/(Complex<T> a, Complex<T> b) {
    T scale = T(1)/b.norm();
    return scale*(a*b.conj());
}

template <typename T>
T squareRoot(T x);

template <>
mpz_class squareRoot<mpz_class>(mpz_class s);

template <>
mpq_class squareRoot<mpq_class>(mpq_class x);

template<>
Complex<mpq_class> squareRoot<Complex<mpq_class>>(Complex<mpq_class> z);