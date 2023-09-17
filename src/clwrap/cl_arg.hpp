#pragma once

#include "cl_buffer.hpp"
#include "cl_executable.hpp"
#include <CL/cl.h>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

namespace clwrap {

template <typename T>
class CLArg {
public:
    CLArg(CLExecutable* kernel, unsigned argIndex, T arg);
    void set(T arg);
private:
    CLExecutable* kernel;
    unsigned argIndex;
};

template <typename T>
CLArg<T>::CLArg(CLExecutable* kernel_, unsigned argIndex_, T arg):
    kernel(kernel_), argIndex(argIndex_)
{
    kernel->setArg<T>(argIndex, arg);
}

template <typename T>
void CLArg<T>::set(T arg) {
    kernel->setArg<T>(argIndex, arg);
}

class CLKernelBufferArg {
public:
    virtual std::shared_ptr<CLEvent> lazySet(std::vector<uint8_t>& data) = 0;
};

template <typename T>
class CLBufferArg: public CLKernelBufferArg {
public:
    CLBufferArg(CLExecutable* kernel, cl_mem_flags clMemFlags, unsigned argIndex);
    CLBufferArg(CLExecutable* kernel, cl_mem_flags clMemFlags, unsigned argIndex,
        std::vector<T>& arg);
    CLBufferArg(CLExecutable* kernel, cl_mem_flags clMemFlags, unsigned argIndex, size_t size);
    CLBufferArg(CLExecutable* kernel, cl_mem_flags clMemFlags, unsigned argIndex,
        std::function<void(std::vector<T>&)> f);
    void get(std::vector<T>& arr);
    std::shared_ptr<CLEvent> getAsyncAfterEvent(std::shared_ptr<CLEvent> event,
        std::shared_ptr<std::vector<T>> arr);
    void set(const std::vector<T>& arg);
    std::shared_ptr<CLEvent> setAsync(const std::vector<T>& arg);
    size_t size();
    void lazy(std::vector<T>& arr);
    std::shared_ptr<CLEvent> lazySet(std::vector<uint8_t>& data) override;
private:
    void resize(size_t size);
    CLExecutable* kernel;
    cl_mem_flags clMemFlags;
    unsigned argIndex;
    std::unique_ptr<CLBuffer<T>> buffer;
};

template <typename T>
CLBufferArg<T>::CLBufferArg(CLExecutable* kernel_, cl_mem_flags clMemFlags_, unsigned argIndex_):
    kernel(kernel_), clMemFlags(clMemFlags_), argIndex(argIndex_) { }

template <typename T>
CLBufferArg<T>::CLBufferArg(CLExecutable* kernel_, cl_mem_flags clMemFlags, unsigned argIndex_,
    std::vector<T>& arg): kernel(kernel_), argIndex(argIndex_),
    buffer(std::make_unique<CLBuffer<T>>(
        kernel->context, kernel->context.defaultQueue, clMemFlags, arg.size()))
{
    buffer->write(arg);
    kernel->setBufferArg(argIndex, buffer.get());
}

template <typename T>
CLBufferArg<T>::CLBufferArg(CLExecutable* kernel_, cl_mem_flags clMemFlags, unsigned argIndex_,
    size_t size): kernel(kernel_), argIndex(argIndex_),
    buffer(std::make_unique<CLBuffer<T>>(
        kernel->context, kernel->context.defaultQueue, clMemFlags, size))
{
    kernel->setBufferArg(argIndex, buffer);
}

template <typename T>
CLBufferArg<T>::CLBufferArg(CLExecutable* kernel_, cl_mem_flags clMemFlags, unsigned argIndex_,
    std::function<void(std::vector<T>&)> f):
    kernel(kernel_), argIndex(argIndex_),
    buffer(std::make_unique<CLBuffer<T>>(
        kernel->context, kernel->context.defaultQueue, clMemFlags, f))
{
    kernel->setBufferArg(argIndex, buffer.get());
}

template <typename T>
void CLBufferArg<T>::get(std::vector<T>& arr) {
    buffer->read(arr);
}

template <typename T>
std::shared_ptr<CLEvent> CLBufferArg<T>::getAsyncAfterEvent(std::shared_ptr<CLEvent> event,
    std::shared_ptr<std::vector<T>> arr)
{
    return buffer->readAsyncAfterEvent(event, arr);
}

template <typename T>
void CLBufferArg<T>::resize(size_t size) {
    if (size == 0) {
        buffer = NULL;
    } else {
        buffer = std::make_unique<CLBuffer<T>>(kernel->context,
            kernel->context.defaultQueue, clMemFlags, size);
        kernel->setBufferArg(argIndex, buffer.get());
    }
}

template <typename T>
void CLBufferArg<T>::set(const std::vector<T>& arg) {
    resize(arg.size());
    buffer->write(arg);
}

template <typename T>
std::shared_ptr<CLEvent> CLBufferArg<T>::setAsync(const std::vector<T>& arg) {
    resize(arg.size());
    return buffer->writeAsync(arg);
}

template <typename T>
size_t CLBufferArg<T>::size() {
    if (buffer == NULL) {
        return 0;
    } else {
        return buffer->length();
    }
}

template <typename T>
void CLBufferArg<T>::lazy(std::vector<T>& arr) {
    auto ptr = reinterpret_cast<uint8_t*>(arr.data());
    size_t len = arr.size()*sizeof(T);
    std::vector<uint8_t> data(ptr, ptr+len);
    kernel->lazyArgs.push_back(CLExecutable::LazyArg(this, data));
}

template <typename T>
std::shared_ptr<CLEvent> CLBufferArg<T>::lazySet(std::vector<uint8_t>& data) {
    if (data.size() == 0) {
        throw std::invalid_argument("Buffer size cannot be zero");
    }
    size_t len = data.size()/sizeof(T);
    resize(len);
    auto typedData = reinterpret_cast<T*>(data.data());
    std::vector<T> vec(typedData, typedData+len);
    return setAsync(vec);
}

template <typename T>
class CLReadOnlyBufferArg: public CLBufferArg<T> {
public:
    CLReadOnlyBufferArg(CLExecutable* kernel, unsigned argIndex);
    CLReadOnlyBufferArg(CLExecutable* kernel, unsigned argIndex, std::vector<T>& arg);
    CLReadOnlyBufferArg(CLExecutable* kernel, unsigned argIndex,
        std::function<void(std::vector<T>&)> f);
};

template <typename T>
CLReadOnlyBufferArg<T>::CLReadOnlyBufferArg(CLExecutable* kernel, unsigned argIndex):
    CLBufferArg<T>(kernel, CL_MEM_READ_ONLY, argIndex) { }

template <typename T>
CLReadOnlyBufferArg<T>::CLReadOnlyBufferArg(CLExecutable* kernel, unsigned argIndex,
    std::vector<T>& arg): CLBufferArg<T>(kernel, CL_MEM_READ_ONLY, argIndex, arg) { }

template <typename T>
CLReadOnlyBufferArg<T>::CLReadOnlyBufferArg(CLExecutable* kernel, unsigned argIndex,
    std::function<void(std::vector<T>&)> f): CLBufferArg<T>(kernel, CL_MEM_READ_ONLY, argIndex, f) { }

template <typename T>
class CLWriteOnlyBufferArg: public CLBufferArg<T> {
public:
    CLWriteOnlyBufferArg(CLExecutable* kernel, unsigned argIndex, size_t size);
};

template <typename T>
CLWriteOnlyBufferArg<T>::CLWriteOnlyBufferArg(CLExecutable* kernel, unsigned argIndex,
    size_t size): CLBufferArg<T>(kernel, CL_MEM_WRITE_ONLY, argIndex, size) { }

template <typename T>
class CLReadWriteBufferArg: public CLBufferArg<T> {
public:
    CLReadWriteBufferArg(CLExecutable* kernel, unsigned argIndex);
    CLReadWriteBufferArg(CLExecutable* kernel, unsigned argIndex, size_t size);
    CLReadWriteBufferArg(CLExecutable* kernel, unsigned argIndex, std::vector<T>& arg);
    CLReadWriteBufferArg(CLExecutable* kernel, unsigned argIndex,
        std::function<void(std::vector<T>&)> f);
};

template <typename T>
CLReadWriteBufferArg<T>::CLReadWriteBufferArg(CLExecutable* kernel, unsigned argIndex):
    CLBufferArg<T>(kernel, CL_MEM_READ_WRITE, argIndex) { }

template <typename T>
CLReadWriteBufferArg<T>::CLReadWriteBufferArg(CLExecutable* kernel, unsigned argIndex,
    size_t size): CLBufferArg<T>(kernel, CL_MEM_READ_WRITE, argIndex, size) { }

template <typename T>
CLReadWriteBufferArg<T>::CLReadWriteBufferArg(CLExecutable* kernel, unsigned argIndex,
    std::vector<T>& arg): CLBufferArg<T>(kernel, CL_MEM_READ_WRITE, argIndex, arg) { }

template <typename T>
CLReadWriteBufferArg<T>::CLReadWriteBufferArg(CLExecutable* kernel, unsigned argIndex,
    std::function<void(std::vector<T>&)> f): CLBufferArg<T>(kernel, CL_MEM_READ_WRITE, argIndex, f) { }

}