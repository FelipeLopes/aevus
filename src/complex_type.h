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

template <typename T>
T maxPart(Complex<T> x);

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
mpq_class maxPart<mpq_class>(Complex<mpq_class> z);

template <>
double toDouble<mpq_class>(mpq_class x);


template <typename T>
T abs(T x) {
    return x > 0 ? x : -x;
}

template <typename T>
T exp(T x, T prec) {
    T ans = 1;
    T num = 1;
    T den = 1;
    T term = num/den;
    int i = 1;
    while (abs<T>(term) > prec) {
        num = num*x;
        den = den*i;
        i++;
        term = num/den;
        ans = ans + term;
    }
    return ans;
}

void printCx(cx z);