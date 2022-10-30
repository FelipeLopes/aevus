#include <gmpxx.h>
#include "scaler.hpp"

template <typename T>
Scaler<T>::Scaler(T iniLogscale, T step, int numSteps, T prec) {

}

template class Scaler<mpq_class>;