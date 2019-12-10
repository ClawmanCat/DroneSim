#pragma once

#include "Core.h"

#include <vector>
#include <string>
#include <string_view>
#include <optional>


namespace DroneSim::Paths {
    constexpr static inline std::string_view PATH_KERNELS = "./kernels/";
}


namespace DroneSim::FileIO {
    using TextFile = std::vector<std::string>;
    using DataFile = std::vector<u8>;


    extern std::optional<TextFile> ReadText(std::string_view path);
    extern bool WriteText(std::string_view path, const TextFile& text);

    extern std::optional<DataFile> ReadData(std::string_view path);
    extern bool WriteData(std::string_view path, const DataFile& data);
}