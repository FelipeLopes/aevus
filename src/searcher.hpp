#pragma once

#include "key_gasket.hpp"
#include "scaler.hpp"
#include "sdf.hpp"
#include "shape.hpp"
#include <array>
#include <boost/asio/thread_pool.hpp>
#include <memory>

template <typename T>
class Searcher {
public:
    Searcher(const Shape<T>& shape,
        const Scaler<T>& scaler,
        Complex<T> center,
        bool inverseDive,
        const std::vector<Mobius<T>>& zoomTransforms,
        std::vector<KeyGasket>& keyGaskets,
        T ar, int numThreads = 4);
    void start();
    void block();
private:
    void task(int i);
    int searchScale(Sdf<T> sdf);
    const Shape<T>& shape;
    std::array<Complex<T>, 3> pts;
    Complex<T> center;
    bool inverseDive;
    std::array<Mobius<T>, 3> transforms;
    T ar;
    int numThreads;
    const Scaler<T>& scaler;
    boost::asio::thread_pool threadPool;
    const std::vector<Mobius<T>>& zoomTransforms;
    std::vector<KeyGasket>& keyGaskets;
    std::mutex lock;
    int lastPickedUp;
    bool foundEnd = false;
};