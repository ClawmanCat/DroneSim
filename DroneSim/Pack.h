#pragma once

#include "Core.h"

namespace DroneSim::Traits {
    template <typename... Args> struct Pack {
        template <template <typename... Ts> typename X> struct Apply {
            using type = X<Args...>;
        };
    };
}