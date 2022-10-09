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
        auto m = su11(1,4,2,3);
        print(m.a);
        print(m.b);
        print(m.c);
        print(m.d);
    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}