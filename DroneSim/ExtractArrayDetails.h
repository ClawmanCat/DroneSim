#pragma once

#include "Core.h"

#include <array>

namespace DroneSim::Traits {
    template <typename T> class ExtractArrayDetails {
    public:
        constexpr static bool is_array = false;
    };


    template <typename T, std::size_t S> class ExtractArrayDetails<std::array<T, S>> {
    public:
        constexpr static bool is_array = true;

        using type = T;
        constexpr static std::size_t size = S;
    };
}