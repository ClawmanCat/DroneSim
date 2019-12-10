#pragma once

#include "Core.h"

#include <type_traits>

namespace DroneSim::Traits {
    // The requirement for being considered a container is that std::begin and std::end can be called on it,
    // and that the iterator produced by std::begin can be incremented.
    template <typename T> class IsContainer {
    private:
        template <typename TT = T> constexpr static auto test(int) -> decltype(
            std::begin(std::declval<TT>()), 
            std::end(std::declval<TT>()), 
            std::next(std::begin(std::declval<TT>())),
            std::true_type()
        );

        template <typename TT = T> constexpr static auto test(...) -> std::false_type;
    public:
        constexpr static bool value = decltype(test(0))::value;
    };
}