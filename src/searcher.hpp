#pragma once

#include "key_gasket.hpp"
#include "scaler.hpp"
#include "sdf.hpp"
#include <array>
#include <boost/asio/thread_pool.hpp>
#include <memory>

template <typename T>
class Searcher {
public:
    Searcher(std::shared_ptr<Scaler<T>> scaler,
        Complex<T> pa, Complex<T> pb, Complex<T> pc,
        Complex<T> center,
        std::array<Mobius<T>, 3> transforms,
        const std::vector<Mobius<T>>& input,
        std::vector<KeyGasket>& output,
        T ar, int numThreads = 4);
private:
    void task(int i);
    int searchScale(Sdf<T> sdf);
    Complex<T> pa, pb, pc;
    Complex<T> center;
    std::array<Mobius<T>, 3> transforms;
    T ar;
    std::shared_ptr<Scaler<T>> scaler;
    boost::asio::thread_pool threadPool;
    const std::vector<Mobius<T>>& input;
    std::vector<KeyGasket>& output;
    std::mutex lock;
    int lastPickedUp;
    bool foundEnd;
};