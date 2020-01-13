#pragma once

#include "Core.h"

namespace DroneSim::Traits {
     template <typename Pred, typename... Args> class TakesOverload {
        template <typename P = Pred> constexpr static auto test(int) -> decltype(std::declval<P>()(std::declval<Args>()...), std::true_type());
        template <typename P = Pred> constexpr static auto test(...) -> std::false_type;
    public:
        constexpr static bool value = decltype(test(0))::value;
    };

    template <typename Pred, typename... Args> constexpr static bool takes_overload_v = TakesOverload<Pred, Args...>::value;
}