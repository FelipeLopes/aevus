#include <complex>
#include <gmpxx.h>
#include "sdf.h"

template <typename T>
Sdf<T>::Sdf() {}

template <typename T>
Sdf<T>::Sdf(T a_, T b_, T c_, T d_):
    a(a_),b(b_),c(c_),d(d_){

}

template <typename T>
bool Sdf<T>::inside(Complex<T> z) {
    T x = z.real;
    T y = z.imag;
    return a*(x*x+y*y) + b*x + c*y + d < 0;
}

template <typename T>
Sdf<T> Sdf<T>::flip() {
    return Sdf(-a,-b,-c,-d);
}

template <typename T>
T Sdf<T>::det33(std::array<std::array<T,3>,3> m) {
    return m[1][1]*m[2][2]*m[3][3]+m[1][2]*m[2][3]*m[3][1]+m[1][3]*m[2][1]*m[3][2]-
        m[1][1]*m[2][3]*m[3][2]-m[1][2]*m[2][1]*m[3][3]-m[1][3]*m[2][2]*m[3][1];
}

template <typename T>
Sdf<T> Sdf<T>::fromPoints(Complex<T> p, Complex<T> q, Complex<T> r) {
    return Sdf<T>();
}

template class Sdf<mpq_class>;