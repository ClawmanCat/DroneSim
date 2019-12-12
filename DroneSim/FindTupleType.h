#pragma once

#include "Core.h"

#include <tuple>
#include <type_traits>

namespace DroneSim::Traits {
    // Find the first element of type T in a tuple.
    template <typename T, typename Tuple, std::size_t Next = 0> constexpr static std::size_t FindTupleType(void) {
        if constexpr (std::is_same_v<T, decltype(std::get<Next>(std::declval<Tuple>()))>) return Next;
        else return FindTupleType<T, Tuple, Next + 1>();
    }
}