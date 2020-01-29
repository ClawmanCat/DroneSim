#pragma once

#include "Core.h"

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

namespace DroneSim::StringUtils {
    template <typename Container> inline std::string cat(const Container& ctr) {
        return std::accumulate(std::begin(ctr), std::end(ctr), std::string());
    }


    template <typename Container> inline std::string cat(const Container& ctr, const char* seperator) {
        return std::accumulate(std::begin(ctr), std::end(ctr), std::string(), [seperator](const std::string& acc, const auto& elem) { return acc + seperator + elem; });
    }


    template <typename... Ts> inline std::string cat(const Ts&... args) {
        std::stringstream result;
        ((result << args), ...);

        return result.str();
    }


    inline std::vector<std::string> split(const std::string& string, char seperator = '\n') {
        std::vector<std::string> result;
        
        std::size_t last = 0;
        for (std::size_t i = 0; i < string.length(); ++i) {
            if (string[i] == seperator) {
                result.push_back(std::string(string.begin() + last, string.begin() + i));
                last = i + 1;
            }
        }
        result.push_back(std::string(string.begin() + last, string.end()));

        return result;
    }
}