#pragma once

template <typename T>
class Sdf {
public:
    Sdf(T a, T b, T c, T d);
private:
    T a, b, c, d;
};