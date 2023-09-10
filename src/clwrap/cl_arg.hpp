#pragma once

#include "cl_buffer.hpp"
#include "cl_executable.hpp"
#include <CL/cl.h>
#include <functional>
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

template <typename T>
class CLBufferArg {
public:
    CLBufferArg(CLExecutable* kernel, cl_mem_flags clMemFlags, unsigned argIndex,
        std::vector<T>& arg);
    CLBufferArg(CLExecutable* kernel, cl_mem_flags clMemFlags, unsigned argIndex, size_t size);
    CLBufferArg(CLExecutable* kernel, cl_mem_flags clMemFlags, unsigned argIndex,
        std::function<void(std::vector<T>&)> f);
    void get(std::vector<T>& arr);
    void set(const std::vector<T>& arg);
private:
    CLExecutable* kernel;
    unsigned argIndex;
    CLBuffer<T> buffer;
};

template <typename T>
CLBufferArg<T>::CLBufferArg(CLExecutable* kernel_, cl_mem_flags clMemFlags, unsigned argIndex_,
    std::vector<T>& arg): kernel(kernel_), argIndex(argIndex_),
    buffer(kernel->context, kernel->context.defaultQueue, clMemFlags, arg.size())
{
    buffer.write(arg);
    kernel->setBufferArg(argIndex, buffer);
}

template <typename T>
CLBufferArg<T>::CLBufferArg(CLExecutable* kernel_, cl_mem_flags clMemFlags, unsigned argIndex_,
    size_t size): kernel(kernel_), argIndex(argIndex_),
    buffer(kernel->context, kernel->context.defaultQueue, clMemFlags, size)
{
    kernel->setBufferArg(argIndex, buffer);
}

template <typename T>
CLBufferArg<T>::CLBufferArg(CLExecutable* kernel_, cl_mem_flags clMemFlags, unsigned argIndex_,
    std::function<void(std::vector<T>&)> f):
    kernel(kernel_),
    argIndex(argIndex_),
    buffer(kernel->context, kernel->context.defaultQueue, clMemFlags, f)
{
    kernel->setBufferArg(argIndex, buffer);
}

template <typename T>
void CLBufferArg<T>::get(std::vector<T>& arr) {
    buffer.read(arr);
}

template <typename T>
void CLBufferArg<T>::set(const std::vector<T>& arg) {
    buffer.write(arg);
}

template <typename T>
class CLReadOnlyBufferArg: public CLBufferArg<T> {
public:
    CLReadOnlyBufferArg(CLExecutable* kernel, unsigned argIndex, std::vector<T>& arg);
    CLReadOnlyBufferArg(CLExecutable* kernel, unsigned argIndex,
        std::function<void(std::vector<T>&)> f);
};

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
    CLReadWriteBufferArg(CLExecutable* kernel, unsigned argIndex, size_t size);
    CLReadWriteBufferArg(CLExecutable* kernel, unsigned argIndex, std::vector<T>& arg);
    CLReadWriteBufferArg(CLExecutable* kernel, unsigned argIndex,
        std::function<void(std::vector<T>&)> f);
};

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