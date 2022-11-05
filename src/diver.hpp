#pragma once

#include "mobius.hpp"
#include <random>

template<typename T>
class Diver {
public:
    Diver() { }
    virtual int chooseDive(Mobius<T> accumulator) = 0;
    virtual int getDepth() const = 0;
    virtual ~Diver() { }
};