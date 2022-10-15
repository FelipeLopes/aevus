#include <complex>
#include <gmpxx.h>
#include "complex_type.h"
#include "sdf.h"

template <typename T>
Sdf<T>::Sdf(T a_, T b_, T c_, T d_):
    a(a_),b(b_),c(c_),d(d_){

    printf("%lf\n",toDouble(a));
    printf("%lf\n",toDouble(b));
    printf("%lf\n",toDouble(c));
    printf("%lf\n",toDouble(d));
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
    return m[0][0]*m[1][1]*m[2][2]+m[0][1]*m[1][2]*m[2][0]+m[0][2]*m[1][0]*m[2][1]-
        m[0][0]*m[1][2]*m[2][1]-m[0][1]*m[1][0]*m[2][2]-m[0][2]*m[1][1]*m[2][0];
}

template <typename T>
Sdf<T> Sdf<T>::fromPoints(Complex<T> p, Complex<T> q, Complex<T> r) {
    T xp = p.real;
    T yp = p.imag;
    T np = xp*xp + yp*yp;
    T xq = q.real;
    T yq = q.imag;
    T nq = xq*xq + yq*yq;
    T xr = r.real;
    T yr = r.imag;
    T nr = xr*xr + yr*yr;

    T a = det33({std::array<T,3>
        {xp, yp, 1},
        {xq, yq, 1},
        {xr, yr, 1}
    });
    T b = -det33({std::array<T,3>
        {np, yp, 1},
        {nq, yq, 1},
        {nr, yr, 1}
    });
    T c = det33({std::array<T,3>
        {np, xp, 1},
        {nq, xq, 1},
        {nr, xr, 1}
    });
    T d = -det33({std::array<T,3>
        {np, xp, yp},
        {nq, xq, yq},
        {nr, xr, yr}
    });
    return Sdf<T>(a, b, c, d);
}

template class Sdf<mpq_class>;