#include <iostream>
#include <gmpxx.h>

#include "complex_type.h"
#include "mobius.h"
#include "gasket.h"

template <typename T>
Gasket<T>::Gasket(T r1, T r2, Complex<T> f, bool flip) {

    if (r2 > r1) {
        throw std::invalid_argument("First radius parameter should be greater than "
            "or equal to the second.");
    } else if (r1 + r2 > 1) {
        throw std::invalid_argument("Radii sum cannot be larger than 1.");
    } else if (r1 < 0 || r2 < 0) {
        throw std::invalid_argument("All radii should be positive.");
    }

    if (f.norm() != 1) {
        throw std::invalid_argument("Phase must have unit norm.");
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
        throw std::invalid_argument("Radii given are not the largest circles.");
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
        Complex<T>(2)*ii,Complex<T>(1)).conjugate(m);
    rot = Mobius<T>::fromPointsToPoints(
        Complex<T>(0), Complex<T>(1), Complex<T>(-1),
        Complex<T>(1), Complex<T>(-1), Complex<T>(0)).conjugate(m);

    tr.normalize();
    rot.normalize();

    center = Complex<T>(0);
    scale = 1;
}

template <typename T>
void Gasket<T>::setCenter(Complex<T> center_) {
    center = center_;
}

template <typename T>
void Gasket<T>::setScale(double scale_) {
    scale = scale_;
}

template <typename T>
Complex<T> Gasket<T>::selectZoomPoint(unsigned seed) {
    return Complex<T>(0);
}

template <typename T>
Mobius<T> Gasket<T>::adapt(Complex<T> p1, Complex<T> p2, Complex<T> p3,
    Mobius<T> dive, Mobius<T> rot) {

    std::array<Mobius<T>, 3> arr = {dive, dive.conjugate(rot),
        dive.conjugate(rot.inverse())};

    return Mobius<T>::scaling(Complex<T>(1));
}

template<typename T>
Flame Gasket<T>::toFlame() {
    Flame flame;
    auto s = Mobius<T>::scaling(Complex<T>(scale)).compose(Mobius<T>::translation(-center));
    flame.xforms.push_back(tr.conjugate(s).toXForm());
    flame.xforms.push_back(tr.conjugate(rot).conjugate(s).toXForm());
    flame.xforms.push_back(tr.conjugate(rot.inverse()).conjugate(s).toXForm());
    flame.xforms.push_back(tr.inverse().conjugate(s).toXForm());
    flame.xforms.push_back(tr.inverse().conjugate(rot).conjugate(s).toXForm());
    flame.xforms.push_back(tr.inverse().conjugate(rot.inverse()).conjugate(s).toXForm());

    for (int i = 0; i < 3; i++) {
        flame.xforms[i].chaos = {1, 1, 1, 0, 0, 0};
        flame.xforms[i+3].chaos = {0, 0, 0, 1, 1, 1};
    }

    for (int i=0; i<6; i++) {
        flame.xforms[i].chaos[i] = 3;
    }

    return flame;
}

int genGasket(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);

    if (argc < 7) {
        printf("Usage: %s [px] [py] [qx] [qy] [rx] [ry]\n", argv[0]);
        return 1;
    }

    //cx p = cx(stod(argv[1]), stod(argv[2]));
    //cx q = cx(stod(argv[3]), stod(argv[4]));
    //cx r = cx(stod(argv[5]), stod(argv[6]));

    return 0;
}

template class Gasket<mpq_class>;