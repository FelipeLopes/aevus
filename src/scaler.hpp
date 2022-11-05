#pragma once

#include <vector>

#include "complex_type.hpp"

template <typename T>
class Scaler {
public:
    Scaler(T iniLogscale_, T step_, int numSteps_, T prec):
        iniLogscale(iniLogscale_), step(step_), numSteps(numSteps_) {

        base = exp<T>(iniLogscale, prec);
        lookup.resize(32-__builtin_clz(numSteps));
        for (int i=0; i<lookup.size(); i++) {
            lookup[i] = exp<T>((1<<i)*step, prec);
        }
    }
    T lookupExp(int n) const {
        T ans = base;
        while (n > 0) {
            int bits = (n & -n);
            ans = ans * lookup[__builtin_ctz(bits)];
            n -= bits;
        }
        return ans;
    }
    const T iniLogscale, step;
    const int numSteps;
private:
    T base;
    std::vector<T> lookup;
};