#pragma once

#include "mobius.hpp"
#include <random>

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
private:
    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist2, dist3;
};