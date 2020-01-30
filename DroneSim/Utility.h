#pragma once

#include <vector>
#include <utility>
#include <limits>
#include <iostream>

namespace DroneSim::Utility {
    template <typename T> inline void print_t(void) {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    }


    template <typename T> inline void swap_erase(std::vector<T>& v, typename std::vector<T>::iterator where) {
        std::swap(*where, v.back());
        v.pop_back();
    }


    template <typename T> constexpr inline auto scale(const T& v, const Vec2<T>& oldrange, const Vec2<T>& newrange) {
        return (((v - oldrange.x) * (newrange.y - newrange.x)) / (oldrange.y - oldrange.x)) + newrange.x;
    }


    template <typename T> inline auto dotself(const T& v) {
        return glm::dot(v, v);
    }


    template <typename T> constexpr inline auto square(const T& v) {
        return v * v;
    }


    template <typename T> constexpr inline bool in_range(const Vec2<T>& v, const Vec2<T>& min, const Vec2<T>& max) {
        return v.x >= min.x && v.x < max.x &&
               v.y >= min.y && v.y < max.y;
    }


    template <typename Pred, typename... Containers> constexpr inline void multi_for_each(const Pred& pred, Containers&&... containers) {
        ([&](auto&& container) {
            for (auto& elem : container) pred(elem);
        }(containers), ...);
    }


    namespace Detail {
        constexpr inline double newton_rhapson(double x, double curr, double prev) {
            return (curr == prev)
                ? curr
                : newton_rhapson(x, 0.5 * (curr + x / curr), curr);
        }
    }

    
    // compile-time version of std::sqrt
    // adapted from https://stackoverflow.com/a/34134071
    constexpr inline double constexpr_sqrt(double x) {
        return x >= 0 && x < std::numeric_limits<double>::infinity()
            ? Detail::newton_rhapson(x, x, 0)
            : std::numeric_limits<double>::quiet_NaN();
    }
}