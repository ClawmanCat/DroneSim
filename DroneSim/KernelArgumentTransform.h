#pragma once

#include "Core.h"
#include "CLBuffer.h"
#include "Pack.h"
#include "IsContainer.h"
#include "ElementType.h"

#include <type_traits>
#include <array>
#include <vector>
#include <deque>

namespace DroneSim::Traits {
    // Container type   -> CLBuffer
    // Fundamental type -> self
    // Other            -> not allowed

    namespace Detail {
        template <typename T> constexpr static auto KernelArgChecker(void) {
            if constexpr (std::is_fundamental_v<T>) return std::declval<T>();
            if constexpr (std::is_pointer_v<T>)     return std::declval<CL::CLBuffer<std::remove_pointer_t<T>>>();
            if constexpr (IsContainer<T>::value)    return std::declval<CL::CLBuffer<typename ElementType<T>::type>>();
        }
    }

    template <typename T> struct KernelArgumentTransform {
        using type = decltype(Detail::KernelArgChecker<T>());
    };
}