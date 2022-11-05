#pragma once

#include "mobius.hpp"
#include <random>

template<typename T>
class Diver {
public:
    Diver(int depth, int seed);
    int chooseDive(Mobius<T> accumulator);
    int depth;
private:
    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist2, dist3;
};