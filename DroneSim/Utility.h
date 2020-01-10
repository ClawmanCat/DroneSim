#pragma once

#include <vector>
#include <utility>

namespace DroneSim::Utility {
    template <typename T> inline void swap_erase(std::vector<T>& v, typename std::vector<T>::iterator where) {
        std::swap(*where, v.back());
        v.pop_back();
    }


    template <typename T> inline T scale(const T& v, const Vec2<T>& oldrange, const Vec2<T>& newrange) {
        return (((v - oldrange.x) * (newrange.y - newrange.x)) / (oldrange.y - oldrange.x)) + newrange.x;
    }
}