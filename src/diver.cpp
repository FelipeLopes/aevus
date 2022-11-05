#include <gmpxx.h>
#include "diver.hpp"

template <typename T>
Diver<T>::Diver(int depth_, int seed): depth(depth_), rng(seed),
    dist2(0,1), dist3(0,2) {

}

template <typename T>
int Diver<T>::chooseDive(Mobius<T> acc) {
    if (acc.a == Complex<T>(1) && acc.b == Complex<T>(0) &&
        acc.c == Complex<T>(0) && acc.d == Complex<T>(1)) {

        int k = dist2(rng);
        return k*3 + dist3(rng);
    }
    return dist3(rng);
}

template class Diver<mpq_class>;