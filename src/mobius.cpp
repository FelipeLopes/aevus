#include "mobius.hpp"

template <>
Mobius<double> Mobius<double>::toMobiusDouble() const {
    return *this;
}