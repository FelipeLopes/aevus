#include "searcher.hpp"
#include <gmpxx.h>
#include <memory>

using std::shared_ptr;

template<typename T>
Searcher<T>::Searcher(shared_ptr<Scaler<T>> scaler_, int numThreads):
    scaler(scaler_), threadPool(numThreads) {

}

template class Searcher<mpq_class>;