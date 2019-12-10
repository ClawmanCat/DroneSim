#pragma once

#include "Core.h"

#include <type_traits>

namespace DroneSim::Traits {
    // What is the type contained in the given container?
    template <typename T> struct ElementType {
        using type = std::remove_reference_t<decltype(*std::begin(std::declval<T>()))>;
    };
}