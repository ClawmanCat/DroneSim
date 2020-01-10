#pragma once

namespace DroneSim::Traits {
    // For passing template-values to lambdas.
    template <typename T, T Value> struct ConstexprArg {
        constexpr static T value = Value;
    };
}