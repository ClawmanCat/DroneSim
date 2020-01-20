#pragma once

#include "Core.h"
#include "LodePNG.h"
#include "FileIO.h"

#include <vector>
#include <string_view>

namespace DroneSim::FileIO {
    // Wrapper around LodePNG
    using RGBA = Vec4ub;

    struct ImageData {
        std::vector<RGBA> pixels;
        u32 w, h;
    };


    inline ImageData LoadPNG(std::string_view path) {
        auto bytes = ReadData(path);
        if (!bytes) terminate("Failed to load PNG image from disk.");

        ImageData result { };
        std::vector<u8> tmp;

        auto error = lodepng::decode(tmp, result.w, result.h, bytes.value());
        if (error) terminate("Failed to decode PNG image.");

        result.pixels.resize(tmp.size() >> 2);
        memcpy(&result.pixels[0], &tmp[0], tmp.size());

        return result;
    }
}