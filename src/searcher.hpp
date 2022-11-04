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
    Searcher(std::shared_ptr<Shape<T>> shape,
        std::shared_ptr<Scaler<T>> scaler,
        Complex<T> center,
        bool inverseDive,
        const std::vector<Mobius<T>>& input,
        std::vector<KeyGasket>& output,
        T ar, int numThreads = 4);
    void start();
    void block();
private:
    void task(int i);
    int searchScale(Sdf<T> sdf);
    std::shared_ptr<Shape<T>> shape;
    Complex<T> pa, pb, pc;
    Complex<T> center;
    bool inverseDive;
    std::array<Mobius<T>, 3> transforms;
    T ar;
    int numThreads;
    std::shared_ptr<Scaler<T>> scaler;
    boost::asio::thread_pool threadPool;
    const std::vector<Mobius<T>>& input;
    std::vector<KeyGasket>& output;
    std::mutex lock;
    int lastPickedUp;
    bool foundEnd;
};