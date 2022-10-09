#include <gmpxx.h>
#include <exception>
#include "complex_type.h"
#include "mobius.h"

template<typename T>
Mobius<T>::Mobius(Complex<T> a_, Complex<T> b_, 
    Complex<T> c_, Complex<T> d_): a(a_),b(b_),c(c_),d(d_) {

    auto det = a*d - b*c;
    auto one = Complex<T>::unit();
    if (det != Complex<T>(one)) {
        throw std::invalid_argument("Mobius matrix does not have unit determinant.");
    }
}

template class Mobius<mpq_class>;