#pragma once

#include "complex_type.hpp"

template <typename T>
class Shape {
public:
    Shape(T r1, T r2, Complex<T> f, bool flip = false);
};