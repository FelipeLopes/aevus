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

Mobius<mpq_class> su11(mpz_class m, mpz_class n, mpz_class p, mpz_class q) {
    mpz_class den = p*p + q*q - m*m - n*n;
    if (den == 0) {
        throw std::invalid_argument("Given numbers form a zero denominator.");
    }
    mpz_class num = 2*m;
    mpz_class g = gcd(num, den);
    if (den < 0) {
        den = -den;
        num = -num;
    }
    mpz_class rnum = num/g;
    mpz_class rden = den/g;
    std::string s = rnum.get_str() + "/" + rden.get_str();
    mpq_class x(s);
    mpq_class qOne(1), qm(m.get_str()), qn(n.get_str()), qp(p.get_str()), qq(q.get_str());
    mpq_class a = qOne + x*qm;
    mpq_class b = x*qn;
    mpq_class c = x*qp;
    mpq_class d = x*qq;

    Complex<mpq_class> u(a, b);
    Complex<mpq_class> v(c, d);
    return Mobius<mpq_class>(u,v,v.conj(),u.conj());
}

template class Mobius<mpq_class>;