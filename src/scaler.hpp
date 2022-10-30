#pragma once

template <typename T>
class Scaler {
public:
    Scaler(T iniLogscale, T step, int numSteps, T prec);
};