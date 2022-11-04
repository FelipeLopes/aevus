#include <gmpxx.h>
#include <stdexcept>
#include "shape.hpp"

using std::invalid_argument;

template <typename T>
Shape<T>::Shape(T r1, T r2, Complex<T> f, bool flip) {
    if (r2 > r1) {
        throw invalid_argument("First radius parameter should be greater than "
            "or equal to the second.");
    } else if (r1 + r2 > 1) {
        throw invalid_argument("Radii sum cannot be larger than 1.");
    } else if (r1 < 0 || r2 < 0) {
        throw invalid_argument("All radii should be positive.");
    }

    if (f.norm() != 1) {
        throw invalid_argument("Phase must have unit norm.");
    }

    T a = -1;
    T b = 1/r1;
    T c = 1/r2;
    T s1 = a + b + c;
    T s2 = a*b + b*c + c*a;

    T e = 2*squareRoot<T>(s2);
    T d = s1 - e;
    if (d <= 0) {
        d =  s1 + e;
    }
    if (d < c) {
        throw invalid_argument("Radii given are not the largest circles.");
    }

    T l1 = r1 + r2;
    T l2 = 1 - r1;
    T l3 = 1 - r2;
    T cosx = (l2*l2+l3*l3-l1*l1)/(2*l2*l3);
    T sinx = squareRoot<T>(1-cosx*cosx);

    Complex<T> ii(0,1);
    Complex<T> p1 = f;
    Complex<T> p2 = p1*(Complex<T>(cosx)+Complex<T>(sinx)*ii);

    Complex<T> v1 = (Complex<T>(1)-Complex<T>(r1))*p1;
    Complex<T> v2 = (Complex<T>(1)-Complex<T>(r2))*p2;
    Complex<T> v3 = (r2*v1+r1*v2)/Complex<T>(r1+r2);

    auto m = Mobius<T>::fromPointsToPoints(
        Complex<T>(1),
        Complex<T>(-1),
        Complex<T>(0),
        p1,p2,v3);

    if (flip) {
        m = m.flip();
    }

    tr = Mobius<T>(Complex<T>(1),Complex<T>(0),
        Complex<T>(0,2),Complex<T>(1)).conjugate(m);
    rot = Mobius<T>::fromPointsToPoints(
        Complex<T>(0), Complex<T>(1), Complex<T>(-1),
        Complex<T>(1), Complex<T>(-1), Complex<T>(0)).conjugate(m);

    pa = m.apply(Complex<T>(-1));
    pb = m.apply(Complex<T>(1));
    pc = m.apply(Complex<T>(0));
}

template class Shape<mpq_class>;