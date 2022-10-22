#pragma once

#include "mobius.h"
#include <vector>

template <typename T>
struct KeyGasket {
    std::vector<Mobius<T>> ifsTransforms;
    T logscale;
};