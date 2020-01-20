#pragma once

#include "Core.h"


namespace DroneSim::Traits {
    // Does S specialize the template T?
    template <template <typename...> typename T, typename S> class IsSpecialization {
        template <typename... Args> constexpr static auto test(T<Args...>*) -> std::true_type;
        constexpr static auto test(...) -> std::false_type;
    public:
        constexpr static bool value = decltype(test(std::declval<S*>()))::value;
    };


    // Same as above, but for classes that take variable template parameters.
    template <typename... Ts> struct IsVariableSpecialization {
        template <template <Ts...> typename T, typename S> class IsSpecialization {
            template <Ts... Args> constexpr static auto test(T<Args...>*) -> std::true_type;
            constexpr static auto test(...) -> std::false_type;
        public:
            constexpr static bool value = decltype(test(std::declval<S*>()))::value;
        };
    };
}