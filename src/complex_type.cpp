#include <gmpxx.h>
#include "complex_type.h"

template<typename T>
Complex<T>::Complex(): real(Complex<T>::zero()), imag(Complex<T>::zero()) {

}

template<typename T>
Complex<T>::Complex(T a): real(a), imag(Complex<T>::zero()) {

}

template<typename T>
Complex<T>::Complex(T a, T b): real(a), imag(b) {

}

template<typename T>
T Complex<T>::norm() {
    return real*real + imag*imag;
}

template<typename T>
Complex<T> Complex<T>::conj() {
    return Complex<T>(real, -imag);
}

template class Complex<mpq_class>;