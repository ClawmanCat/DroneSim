#pragma once

#include "Core.h"

#include <type_traits>

namespace DroneSim {
    template <template <typename... Ts> typename Container, typename... Stored> using VariadicStorage = std::tuple<Container<Stored>...>;
}