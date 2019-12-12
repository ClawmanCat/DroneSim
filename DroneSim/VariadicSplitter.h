#pragma once

#include "Core.h"
#include "Pack.h"

namespace DroneSim::Traits {
    template <typename H, typename... T> struct VariadicSplitter {
        using head = H;
        using tail = Pack<T...>;
    };
}