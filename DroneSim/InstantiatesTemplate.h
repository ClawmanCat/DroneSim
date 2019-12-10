#pragma once

#include "Core.h"

#include <type_traits>

namespace DroneSim::Traits {
    // Is A a specialization of template X?
    template <typename A, template <typename... Args> typename X> class InstantiatesTemplate {
    private:
        template <typename... Ts> struct Test              { using type = std::false_type; };
        template <typename... Ts> struct Test<X<Ts...>>    { using type = std::true_type;  };
    public:
        constexpr static bool value = Test<A>::type::value;
    };
}