#pragma once

#include <gmpxx.h>
#include "complex_type.hpp"
#include "xform.hpp"

namespace gasket {

template <typename T>
class Mobius {
public:
    Mobius(): a(Complex<T>(1)),b(Complex<T>(0)),
        c(Complex<T>(0)),d(Complex<T>(1)) {

    }

    Mobius(Complex<T> a_, Complex<T> b_, Complex<T> c_, Complex<T> d_):
        a(a_),b(b_),c(c_),d(d_) {

        auto det = a*d - b*c;
        if (det == Complex<T>(0)) {
            throw std::invalid_argument("Mobius matrix is singular.");
        }
    }

    Complex<T> a, b, c, d;

    Complex<T> apply(Complex<T> z) const {
        return (a*z+b)/(c*z+d);
    }

    Mobius<T> inverse() const {
        Complex<T> sc = Complex<T>(1)/(a*d-b*c);
        return Mobius<T>(sc*d,-sc*b,-sc*c,sc*a);
    }

    Mobius<T> compose(Mobius<T> n) const {
        return Mobius<T>(a*n.a + b*n.c, a*n.b + b*n.d, c*n.a + d*n.c, c*n.b + d*n.d);
    }

    Mobius<T> conjugate(Mobius<T> s) const {
        return s.compose(*this).compose(s.inverse());
    }

    Mobius<T> flip() const {
        return Mobius<T>(a.conj(),b.conj(),c.conj(),d.conj());
    }

    Mobius<double> toMobiusDouble() const {
        return Mobius<double>(a.toComplexDouble(),b.toComplexDouble(),
            c.toComplexDouble(),d.toComplexDouble());
    }

    void normalize() {
        Complex<T> sdet = squareRoot<Complex<T>>(a*d - b*c);
        a = a / sdet;
        b = b / sdet;
        c = c / sdet;
        d = d / sdet;
    }

    XForm toXForm(double colorValue) const {
        if (c == Complex<T>(0)) {
            auto o = apply(Complex<T>(0));
            auto x = apply(Complex<T>(1))-o;
            auto y = apply(Complex<T>(0,1))-o;

            cx of = o.toCxDouble();
            cx xf = x.toCxDouble();
            cx yf = y.toCxDouble();

            return XForm(
                XForm::LINEAR,
                Affine(
                    xf.real(), xf.imag(),
                    yf.real(), yf.imag(),
                    of.real(), of.imag()
                ),
                Affine(),
                colorValue
            );
        } else {
            auto preInv = Mobius<T>::translation(d/c);

            auto preO = preInv.apply(Complex<T>(0)).conj();
            auto preX = preInv.apply(Complex<T>(1)).conj()-preO;
            auto preY = preInv.apply(Complex<T>(0,1)).conj()-preO;

            cx preOf = preO.toCxDouble();
            cx preXf = preX.toCxDouble();
            cx preYf = preY.toCxDouble();

            auto pstInv = Mobius<T>::translation(a/c).compose(
                Mobius<T>::scaling(-Complex<T>(1)/(c*c))
            );

            auto pstO = pstInv.apply(Complex<T>(0));
            auto pstX = pstInv.apply(Complex<T>(1))-pstO;
            auto pstY = pstInv.apply(Complex<T>(0,1))-pstO;

            cx pstOf = pstO.toCxDouble();
            cx pstXf = pstX.toCxDouble();
            cx pstYf = pstY.toCxDouble();

            return XForm(XForm::SPHERICAL,
                Affine(
                    preXf.real(),preXf.imag(),
                    preYf.real(),preYf.imag(),
                    preOf.real(),preOf.imag()
                ),
                Affine(
                    pstXf.real(),pstXf.imag(),
                    pstYf.real(),pstYf.imag(),
                    pstOf.real(),pstOf.imag()
                ),
                colorValue
            );
        }
    }

    static Mobius<T> scaling(Complex<T> a) {
        return Mobius<T>(a, Complex<T>(0), Complex<T>(0), Complex<T>(1));
    }

    static Mobius<T> translation(Complex<T> b) {
        return Mobius<T>(Complex<T>(1), b, Complex<T>(0), Complex<T>(1));
    }

    static Mobius<T> fromPoints(Complex<T> p, Complex<T> q, Complex<T> r) {
        return Mobius<T>(q-r,-p*(q-r),(q-p),-r*(q-p));
    }

    static Mobius<T> fromPointsToPoints(Complex<T> p1, Complex<T> q1,
        Complex<T> r1, Complex<T> p2, Complex<T> q2, Complex<T> r2) {

        return fromPoints(p2, q2, r2).inverse().compose(fromPoints(p1, q1, r1));
    }
};

}