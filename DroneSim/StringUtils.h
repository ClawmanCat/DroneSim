#pragma once

#include "Core.h"

#include <algorithm>
#include <string>

namespace DroneSim::StringUtils {
    template <typename Container> inline std::string cat(const Container& ctr) {
        return std::accumulate(std::begin(ctr), std::end(ctr), std::string());
    }

    template <typename... Ts> inline std::string cat(const Ts&... ts) {
        return (std::string(args) + ...);
    }
}