#include <gmpxx.h>
#include "sdf.h"

template <typename T>
Sdf<T>::Sdf(T a_, T b_, T c_, T d_):
    a(a_),b(b_),c(c_),d(d_){

}

template class Sdf<mpq_class>;