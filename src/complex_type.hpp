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
    Complex<T> conj() const;
    cx toCxDouble();
    Complex<double> toComplexDouble() const;
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

template <typename T>
double toDouble(T x);

template <typename T>
T abs(T x);

template <typename T>
T exp(T x, T prec);

template <>
mpz_class squareRoot<mpz_class>(mpz_class s);

template <>
mpq_class squareRoot<mpq_class>(mpq_class x);

template <>
Complex<mpq_class> squareRoot<Complex<mpq_class>>(Complex<mpq_class> z);

template <>
double toDouble<mpq_class>(mpq_class x);


template <typename T>
T abs(T x) {
    return x > 0 ? x : -x;
}

template <typename T>
T exp(T x, T prec) {
    T c = 1;
    T d = 0;
    T f = 1;
    c = 2-x + (2*x)/c;
    d = 1/(2-x+(2*x)*d);
    T x2 = x*x;
    T b = 6;
    T nf = c*d*f;
    while (abs<T>(nf-f) > prec) {
        f = nf;
        c = b + x2/c;
        d = 1/(b+x2*d);
        b = b + 4;
        nf = c*d*f;
    }
    return nf;
}

void printCx(cx z);