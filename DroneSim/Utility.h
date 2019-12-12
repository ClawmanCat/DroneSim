#pragma once

#include <vector>
#include <utility>

namespace DroneSim::Utility {
    template <typename T> inline void swap_erase(std::vector<T>& v, typename std::vector<T>::iterator where) {
        std::swap(*where, v.back());
        v.pop_back();
    }
}