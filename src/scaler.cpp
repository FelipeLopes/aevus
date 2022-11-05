#include <gmpxx.h>
#include "complex_type.hpp"
#include "scaler.hpp"

template <typename T>
Scaler<T>::Scaler(T iniLogscale_, T step_, int numSteps_, T prec):
    iniLogscale(iniLogscale_), step(step_), numSteps(numSteps_) {

    base = exp<T>(iniLogscale, prec);
    lookup.resize(32-__builtin_clz(numSteps));
    for (int i=0; i<lookup.size(); i++) {
        lookup[i] = exp<T>((1<<i)*step, prec);
    }
}

template <typename T>
T Scaler<T>::lookupExp(int n) const {
    T ans = base;
    while (n > 0) {
        int bits = (n & -n);
        ans = ans * lookup[__builtin_ctz(bits)];
        n -= bits;
    }
    return ans;
}

template class Scaler<mpq_class>;