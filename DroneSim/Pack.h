#pragma once

#include "Core.h"
#include "VariadicStorage.h"

namespace DroneSim::Traits {
    template <typename... Args> struct Pack {
        template <template <typename... Ts> typename X> struct Apply {
            using type = X<Args...>;
        };    
    };
}