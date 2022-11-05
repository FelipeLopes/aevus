#pragma once

#include "mobius.hpp"
#include <random>

template<typename T>
class Diver {
public:
    Diver(int depth_, int seed): depth(depth_), rng(seed),
        dist2(0,1), dist3(0,2) {

    }
    int chooseDive(Mobius<T> acc) {
        if (acc.a == Complex<T>(1) && acc.b == Complex<T>(0) &&
            acc.c == Complex<T>(0) && acc.d == Complex<T>(1)) {

            int k = dist2(rng);
            return k*3 + dist3(rng);
        }
        return dist3(rng);
    }
    int depth;
private:
    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist2, dist3;
};