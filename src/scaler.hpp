#pragma once

#include <vector>

template <typename T>
class Scaler {
public:
    Scaler(T iniLogscale, T step, int numSteps, T prec);
    T lookupExp(int n);
private:
    T base;
    std::vector<T> lookup;
};