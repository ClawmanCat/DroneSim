#pragma once

#include "Core.h"

namespace DroneSim::Traits {
    template <typename T> class ExtractVectorDetails {
    public:
        constexpr static bool is_vector = false;
        constexpr static bool is_matrix = false;

        using type = T;
    };


    template <typename T, std::size_t S> class ExtractVectorDetails<glm::vec<S, T>> {
    public:
        constexpr static bool is_vector = true;
        constexpr static bool is_matrix = false;

        using type = T;

        constexpr static std::size_t size = S;
    };


    template <typename T, std::size_t X, std::size_t Y> class ExtractVectorDetails<glm::mat<X, Y, T>> {
    public:
        constexpr static bool is_vector = false;
        constexpr static bool is_matrix = true;

        using type = T;

        constexpr static std::size_t columns = X;
        constexpr static std::size_t rows = Y;
    };




    template <typename T> constexpr static std::size_t GetCount(void) {
        using details = ExtractVectorDetails<T>;

        if constexpr (details::is_vector) return details::size;
        if constexpr (details::is_matrix) return details::rows * details::columns;
        else return 1;
    }
}