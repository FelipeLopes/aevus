#pragma once

#include "scaler.hpp"
#include <boost/asio/thread_pool.hpp>
#include <memory>

template <typename T>
class Searcher {
public:
    Searcher(std::shared_ptr<Scaler<T>> scaler, int numThreads = 4);
private:
    std::shared_ptr<Scaler<T>> scaler;
    boost::asio::thread_pool threadPool;
};