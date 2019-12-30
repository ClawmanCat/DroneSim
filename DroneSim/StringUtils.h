#pragma once

#include "Core.h"

#include <algorithm>
#include <string>

namespace DroneSim::StringUtils {
    template <typename Container> inline std::string cat(const Container& ctr) {
        return std::accumulate(std::begin(ctr), std::end(ctr), std::string());
    }

    template <typename Container> inline std::string cat(const Container& ctr, const char* seperator) {
        return std::accumulate(std::begin(ctr), std::end(ctr), std::string(), [seperator](const std::string& acc, const auto& elem) { return acc + seperator + elem; });
    }

    template <typename... Ts> inline std::string cat(const Ts&... args) {
        return (std::string(args) + ...);
    }
}