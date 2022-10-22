#pragma once

#include "flame.h"
#include "mobius.h"
#include <vector>

template <typename T>
class KeyGasket {
public:
    std::vector<Mobius<T>> ifsTransforms;
    T logscale;
    Flame toFlame();
};