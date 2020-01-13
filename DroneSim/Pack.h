#pragma once

#include "Core.h"

namespace DroneSim::Traits {
    template <typename... Args> class Pack {
    public:
        template <template <typename...> typename X> struct Apply {
            using type = X<Args...>;
        };  


        template <typename T> constexpr static bool Contains(void) {
            return ContainsImpl<T, Args...>();
        }


        template <typename... Ts> constexpr static bool ContainsAll(void) {
            return ContainsAllImpl<Ts...>();
        }


        template <typename T> using Prepend = Pack<T, Args...>;
    private:
        template <typename T, typename Head, typename... Tail> constexpr static bool ContainsImpl(void) {
            if constexpr (std::is_same_v<T, Head>) return true;

            if constexpr (sizeof...(Tail) == 0) return false;
            else return ContainsImpl<T, Tail...>();
        }


        template <typename Head, typename... Tail> constexpr static bool ContainsAllImpl(void) {
            if constexpr (!Contains<Head>()) return false;

            if constexpr (sizeof...(Tail) == 0) return true;
            else return ContainsAllImpl<Tail...>();
        }
    };
}