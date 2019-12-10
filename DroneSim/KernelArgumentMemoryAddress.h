#pragma once

#include "Core.h"
#include "CLBuffer.h"

#include <CL/cl.h>

#include <cstddef>

namespace DroneSim::Traits {
    // CLBuffer         -> Address of underlying cl_mem
    // Fundamental type -> Address of self
    template <typename T> struct KernelArgumentMemoryAddress {
        static_assert(std::is_fundamental_v<T>);

        constexpr static std::size_t size   = sizeof(T);
        constexpr static std::size_t offset = 0;
    };

    template <typename T> struct KernelArgumentMemoryAddress<CL::CLBuffer<T>> {
        constexpr static std::size_t size   = sizeof(cl_mem);
        constexpr static std::size_t offset = CL::CLBuffer<T>::storage_offset();
    };
}