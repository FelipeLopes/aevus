#include <gmpxx.h>
#include "complex_type.h"

template <typename T>
class Mobius {
public:
    Mobius(Complex<T> a, Complex<T> b, Complex<T> c, Complex<T> d);
    Complex<T> a, b, c, d;
    Mobius<T> inverse();
    Mobius<T> compose(Mobius<T> n);
    Mobius<T> conjugate(Mobius<T> s);
};

Mobius<mpq_class> su11(mpz_class m, mpz_class n, mpz_class p, mpz_class q);