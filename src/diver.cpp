#include <gmpxx.h>
#include "diver.hpp"

template <typename T>
Diver<T>::Diver(int depth_):depth(depth_) {

}

template <typename T>
RandomDiver<T>::RandomDiver(int depth, int seed): Diver<T>(depth) {

}

template <typename T>
int RandomDiver<T>::chooseDive(Mobius<T> accumulator) {
    return 0;
}

template class Diver<mpq_class>;
template class RandomDiver<mpq_class>;