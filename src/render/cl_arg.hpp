#pragma once

#include "cl_buffer.hpp"
#include "cl_executable.hpp"
#include <CL/cl.h>
#include <functional>
#include <vector>

namespace render {

template <typename T>
class CLArg {
public:
    CLArg(CLExecutable& kernel, unsigned argIndex, T arg);
};

template <typename T>
CLArg<T>::CLArg(CLExecutable& kernel, unsigned argIndex, T arg) {
    kernel.setArg<T>(argIndex, arg);
}

template <typename T>
class CLBufferArg {
public:
    CLBufferArg(CLExecutable& kernel, cl_mem_flags clMemFlags, unsigned argIndex,
        std::vector<T>& arg);
    CLBufferArg(CLExecutable& kernel, cl_mem_flags clMemFlags, unsigned argIndex, size_t size);
    CLBufferArg(CLExecutable& kernel, cl_mem_flags clMemFlags, unsigned argIndex,
        std::function<void(std::vector<T>&)> f);
    CLBuffer<T> buffer;
};

template <typename T>
CLBufferArg<T>::CLBufferArg(CLExecutable& kernel, cl_mem_flags clMemFlags, unsigned argIndex,
    std::vector<T>& arg): buffer(kernel.context, kernel.context.defaultQueue, clMemFlags, arg.size())
{
    buffer.write(arg);
    kernel.setBufferArg(argIndex, buffer);
}

template <typename T>
CLBufferArg<T>::CLBufferArg(CLExecutable& kernel, cl_mem_flags clMemFlags, unsigned argIndex,
    size_t size): buffer(kernel.context, kernel.context.defaultQueue, clMemFlags, size)
{
    kernel.setBufferArg(argIndex, buffer);
}

template <typename T>
CLBufferArg<T>::CLBufferArg(CLExecutable& kernel, cl_mem_flags clMemFlags, unsigned argIndex,
    std::function<void(std::vector<T>&)> f):
    buffer(kernel.context, kernel.context.defaultQueue, clMemFlags, f)
{
    kernel.setBufferArg(argIndex, buffer);
}

template <typename T>
class CLReadOnlyBufferArg: public CLBufferArg<T> {
public:
    CLReadOnlyBufferArg(CLExecutable& kernel, unsigned argIndex, std::vector<T>& arg);
    CLReadOnlyBufferArg(CLExecutable& kernel, unsigned argIndex,
        std::function<void(std::vector<T>&)> f);
};

template <typename T>
CLReadOnlyBufferArg<T>::CLReadOnlyBufferArg(CLExecutable& kernel, unsigned argIndex,
    std::vector<T>& arg): CLBufferArg<T>(kernel, CL_MEM_READ_ONLY, argIndex, arg) { }

template <typename T>
CLReadOnlyBufferArg<T>::CLReadOnlyBufferArg(CLExecutable& kernel, unsigned argIndex,
    std::function<void(std::vector<T>&)> f): CLBufferArg<T>(kernel, CL_MEM_READ_ONLY, argIndex, f) { }

template <typename T>
class CLWriteOnlyBufferArg: public CLBufferArg<T> {
public:
    CLWriteOnlyBufferArg(CLExecutable& kernel, unsigned argIndex, size_t size);
};

template <typename T>
CLWriteOnlyBufferArg<T>::CLWriteOnlyBufferArg(CLExecutable& kernel, unsigned argIndex,
    size_t size): CLBufferArg<T>(kernel, CL_MEM_WRITE_ONLY, argIndex, size) { }

template <typename T>
class CLReadWriteBufferArg: public CLBufferArg<T> {
public:
    CLReadWriteBufferArg(CLExecutable& kernel, unsigned argIndex, size_t size);
    CLReadWriteBufferArg(CLExecutable& kernel, unsigned argIndex, std::vector<T>& arg);
    CLReadWriteBufferArg(CLExecutable& kernel, unsigned argIndex,
        std::function<void(std::vector<T>&)> f);
};

template <typename T>
CLReadWriteBufferArg<T>::CLReadWriteBufferArg(CLExecutable& kernel, unsigned argIndex,
    size_t size): CLBufferArg<T>(kernel, CL_MEM_READ_WRITE, argIndex, size) { }

template <typename T>
CLReadWriteBufferArg<T>::CLReadWriteBufferArg(CLExecutable& kernel, unsigned argIndex,
    std::vector<T>& arg): CLBufferArg<T>(kernel, CL_MEM_READ_WRITE, argIndex, arg) { }

template <typename T>
CLReadWriteBufferArg<T>::CLReadWriteBufferArg(CLExecutable& kernel, unsigned argIndex,
    std::function<void(std::vector<T>&)> f): CLBufferArg<T>(kernel, CL_MEM_READ_WRITE, argIndex, f) { }

}