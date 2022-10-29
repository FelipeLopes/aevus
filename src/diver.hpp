#pragma once

#include "mobius.hpp"

template<typename T>
class Diver {
public:
    Diver(int depth);
    virtual int chooseDive(Mobius<T> accumulator) = 0;
    const int depth;
    virtual ~Diver() {}
};

template<typename T>
class RandomDiver : public Diver<T> {
public:
    RandomDiver(int depth, int seed);
    int chooseDive(Mobius<T> accumulator);
    ~RandomDiver() {}
};