#include <gmpxx.h>
#include "complex_type.h"
#include "mobius.h"

void print(Complex<mpq_class> c) {
    printf("(%s/%s)+(%s/%s)*i\n", 
        c.real.get_num().get_str().c_str(),
        c.real.get_den().get_str().c_str(),
        c.imag.get_num().get_str().c_str(),
        c.imag.get_den().get_str().c_str());
}

void print(mpq_class k) {
    printf("%s/%s\n",
        k.get_num().get_str().c_str(),
        k.get_den().get_str().c_str());
}

int main() {
    try {
        Complex<mpq_class> p1(mpq_class(1));
        Complex<mpq_class> p2(mpq_class(-4,5),mpq_class(3,5));
        Complex<mpq_class> p3(mpq_class(-7,125),mpq_class(24,125));
        auto m = Mobius<mpq_class>::fromPointsToPoints(
            Complex<mpq_class>(1),
            Complex<mpq_class>(-1),
            Complex<mpq_class>(0),
            p1,p2,p3);
        print(m.a);
        print(m.b);
        print(m.c);
        print(m.d);
    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}