#include "complex_type.h"

template <typename T>
class Mobius {
public:
    Mobius(Complex<T> a, Complex<T> b, Complex<T> c, Complex<T> d);
    Complex<T> a, b, c, d;
};