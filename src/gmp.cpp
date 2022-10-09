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
    printf("Hello GMP\n");
    mpq_class zero(0);
    mpq_class one(1);
    Complex<mpq_class> cOne(one, zero);
    Complex<mpq_class> cI(zero, one);
    mpq_class three(3);
    mpq_class four(4);
    Complex<mpq_class> k = three*cOne + four*cI;
    print(cOne/k);
    try {
        Mobius<mpq_class> m(k,k,k,k);
    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}