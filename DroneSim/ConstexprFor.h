#pragma once

#include "Core.h"
#include "TakesOverload.h"
#include "ConstexprArg.h"

namespace DroneSim::Traits {
    template <std::size_t Count, typename Pred, std::size_t N = 0> constexpr inline void for_constexpr(const Pred& pred) {
        auto call_pred = [&]() constexpr {
            if constexpr (takes_overload_v<Pred>) pred();
            if constexpr (takes_overload_v<Pred, ConstexprArg<std::size_t, N>>) pred(ConstexprArg<std::size_t, N>());
        };

        if constexpr (N < Count) {
            call_pred();
            for_constexpr<Count, Pred, N + 1>(pred);
        }
    }
}