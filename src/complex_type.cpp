#include <gmpxx.h>
#include <iostream>
#include "complex_type.h"

template <typename T>
Complex<T>::Complex(): real(T(0)), imag(T(0)) {

}

template <>
Complex<double>::Complex(): real(0), imag(0) {

}

template <typename T>
Complex<T>::Complex(T a): real(a), imag(T(0)) {

}

template <>
Complex<double>::Complex(double a): real(a), imag(0) {

}

template <typename T>
Complex<T>::Complex(T a, T b): real(a), imag(b) {

}

template <>
Complex<double>::Complex(double a, double b): real(a), imag(b) {

}

template <typename T>
T Complex<T>::norm() {
    return real*real + imag*imag;
}

template <>
double Complex<double>::norm() {
    return real*real + imag*imag;
}

template <typename T>
Complex<T> Complex<T>::conj() {
    return Complex<T>(real, -imag);
}

template <>
Complex<double> Complex<double>::conj() {
    return Complex<double>(real, -imag);
}

template <>
cx Complex<mpq_class>::toCxDouble() {
    mpf_class a(real);
    mpf_class b(imag);
    return a.get_d() + 1i*b.get_d();
}

template <>
cx Complex<double>::toCxDouble() {
    return cx(real, imag);
}

template <>
double toDouble<mpq_class>(mpq_class x) {
    mpf_class f(x);
    return f.get_d();
}

template <>
void Complex<mpq_class>::print() {
    printf("(%s)+i*(%s)\n",real.get_str().c_str(),imag.get_str().c_str());
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

template<>
Complex<double> squareRoot<Complex<double>>(Complex<double> z) {
    cx u = sqrt(cx(z.real,z.imag));
    return Complex<double>(u.real(), u.imag());
}

template <>
mpq_class maxPart<mpq_class>(Complex<mpq_class> z) {
    auto absReal = z.real > 0 ? z.real : -z.real;
    auto absImag = z.imag > 0 ? z.imag : -z.imag;
    return absReal > absImag ? absReal : absImag;
}

void printCx(cx z) {
    std::cout<<"("<<z.real()<<")+i*("<<z.imag()<<")"<<std::endl;
}

template <>
Complex<double> Complex<double>::toComplexDouble() {
    return *this;
}

template <typename T>
Complex<double> Complex<T>::toComplexDouble() {
    return Complex<double>(toDouble(real), toDouble(imag));
}

template class Complex<mpq_class>;