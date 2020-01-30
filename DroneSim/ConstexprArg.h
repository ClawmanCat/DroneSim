#pragma once

namespace DroneSim::Traits {
    // For passing template-values to lambdas.
    template <typename T, T Value> struct ConstexprArg {
        using type = T;
        constexpr static T value = Value;
    };
}