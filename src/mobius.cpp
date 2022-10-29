#include <gmpxx.h>
#include <exception>
#include "complex_type.hpp"
#include "mobius.hpp"
#include "xform.hpp"

template <typename T>
Mobius<T>::Mobius(): a(Complex<T>(1)),b(Complex<T>(0)),
    c(Complex<T>(0)),d(Complex<T>(1)) {

}

template <typename T>
Mobius<T>::Mobius(Complex<T> a_, Complex<T> b_,
    Complex<T> c_, Complex<T> d_): a(a_),b(b_),c(c_),d(d_) {

    auto det = a*d - b*c;
    if (det == Complex<T>(0)) {
        throw std::invalid_argument("Mobius matrix is singular.");
    }
}

template <typename T>
Complex<T> Mobius<T>::apply(Complex<T> z) {
    return (a*z+b)/(c*z+d);
}

template <typename T>
Mobius<T> Mobius<T>::scaling(Complex<T> a) {
    return Mobius<T>(a, Complex<T>(0), Complex<T>(0), Complex<T>(1));
}

template <typename T>
Mobius<T> Mobius<T>::translation(Complex<T> b) {
    return Mobius<T>(Complex<T>(1), b, Complex<T>(0), Complex<T>(1));
}

template <typename T>
Mobius<T> Mobius<T>::fromPoints(Complex<T> p, Complex<T> q, Complex<T> r) {
    return Mobius<T>(q-r,-p*(q-r),(q-p),-r*(q-p));
}

template <typename T>
Mobius<T> Mobius<T>::fromPointsToPoints(Complex<T> p1, Complex<T> q1,
    Complex<T> r1, Complex<T> p2, Complex<T> q2, Complex<T> r2) {

    return fromPoints(p2, q2, r2).inverse().compose(fromPoints(p1, q1, r1));
}

template <typename T>
Mobius<T> Mobius<T>::inverse() {
    Complex<T> sc = Complex<T>(1)/(a*d-b*c);
    return Mobius<T>(sc*d,-sc*b,-sc*c,sc*a);
}

template <typename T>
Mobius<T> Mobius<T>::flip() {
    return Mobius<T>(a.conj(),b.conj(),c.conj(),d.conj());
}

template <typename T>
void Mobius<T>::normalize() {
    Complex<T> sdet = squareRoot<Complex<T>>(a*d - b*c);
    a = a / sdet;
    b = b / sdet;
    c = c / sdet;
    d = d / sdet;
}

template <typename T>
Mobius<T> Mobius<T>::compose(Mobius<T> n) {
    return Mobius<T>(a*n.a + b*n.c, a*n.b + b*n.d, c*n.a + d*n.c, c*n.b + d*n.d);
}

template <typename T>
Mobius<T> Mobius<T>::conjugate(Mobius<T> s) {
    return s.compose(*this).compose(s.inverse());
}

template <typename T>
XForm Mobius<T>::toXForm() {
    if (c == Complex<T>(0)) {
        auto o = apply(Complex<T>(0));
        auto x = apply(Complex<T>(1))-o;
        auto y = apply(Complex<T>(0,1))-o;

        cx of = o.toCxDouble();
        cx xf = x.toCxDouble();
        cx yf = y.toCxDouble();

        return XForm(
            "linear",
            Affine(
                xf.real(), xf.imag(),
                yf.real(), yf.imag(),
                of.real(), of.imag()
            )
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

        return XForm("spherical",
            Affine(
                preXf.real(),preXf.imag(),
                preYf.real(),preYf.imag(),
                preOf.real(),preOf.imag()
            ),
            Affine(
                pstXf.real(),pstXf.imag(),
                pstYf.real(),pstYf.imag(),
                pstOf.real(),pstOf.imag()
            )
        );
    }
}

template <>
Mobius<double> Mobius<double>::toMobiusDouble() {
    return *this;
}

template <typename T>
Mobius<double> Mobius<T>::toMobiusDouble() {
    return Mobius<double>(a.toComplexDouble(),b.toComplexDouble(),
        c.toComplexDouble(),d.toComplexDouble());
}

template class Mobius<mpq_class>;
template class Mobius<double>;