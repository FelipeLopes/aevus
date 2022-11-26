#pragma once

#include "cl_executable.hpp"

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

}