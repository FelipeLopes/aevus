#include <gmpxx.h>
#include "gasket.h"
#include "complex_type.h"
#include "mobius.h"

int main() {
    try {
        Gasket<mpq_class> g(mpq_class(6,11), mpq_class(3,7), Complex<mpq_class>(1));
    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}