#pragma once

#include "Core.h"

#include <cstddef>

namespace DroneSim::GPU::CLGroupSize {
    constexpr static std::size_t WORK_GROUP_EXP  = 5;
    constexpr static std::size_t WORK_GROUP_SIZE = 1 << WORK_GROUP_EXP;

    // Global work size must be a multiple of local work size. Since this is hard to guarantee with random data,
    // we can just find the next multiple of the local work size, and pad all our buffers to that size.
    // While for kernels that write from one buffer to another, at the same index, this doesn't cause any issues,
    // more complicated kernels should make sure they don't access this data and corrupt the result.
    constexpr inline std::size_t GetNextValidGlobalWorkCount(std::size_t size) {
        return (((size - 1) >> WORK_GROUP_EXP) + 1) << WORK_GROUP_EXP;
    }
}