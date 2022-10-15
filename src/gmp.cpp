#include <gmpxx.h>
#include "gasket.h"
#include "complex_type.h"
#include "mobius.h"

void print(mpq_class k) {
    printf("%s/%s\n",
        k.get_num().get_str().c_str(),
        k.get_den().get_str().c_str());
}

int main() {
    try {
        Gasket<mpq_class> g(mpq_class(6,11), mpq_class(3,7), Complex<mpq_class>(1));
        auto xForm = g.rot.toXForm();
        printf("%s\n", xForm.coefsString().c_str());
        printf("%s\n", xForm.postString().c_str());
    } catch (std::exception& e) {
        printf("Error occured: %s\n",e.what());
    }
    return 0;
}