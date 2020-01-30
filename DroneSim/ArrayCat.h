#pragma once

#include "Core.h"

#include <type_traits>
#include <array>

namespace DroneSim::Traits {
    // Adapted from https://stackoverflow.com/a/42749304
    namespace Detail {
        template <std::size_t Last = 0, typename TF, typename TArray, typename... TRest>
        constexpr auto with_acc_sizes(TF&& f, const TArray& array, const TRest&... rest) {
            f(array, std::integral_constant<std::size_t, Last>{});

            if constexpr (sizeof...(TRest) != 0) with_acc_sizes<Last + std::tuple_size_v<TArray>>(f, rest...);
        }
    }


    template<typename T, std::size_t... Sizes> constexpr auto array_cat(const std::array<T, Sizes>&... arrays) {
        std::array<T, (Sizes + ...)> result{};

        Detail::with_acc_sizes([&](const auto& arr, auto offset) {
            std::copy(arr.begin(), arr.end(), result.begin() + offset);
        }, arrays...);

        return result;
    }
}