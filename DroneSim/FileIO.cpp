#include "FileIO.h"

#include <fstream>
#include <filesystem>

namespace DroneSim::FileIO {
    static bool exists(std::string_view path) {
        return std::filesystem::exists(path);
    }


    // Get size of file. Assumes the current get position is the start of the file.
    template <typename Stream> static std::size_t file_size(Stream& stream) {
        auto begpos = stream.tellg();
        stream.seekg(0, std::ios::end);
        auto endpos = stream.tellg();
        stream.seekg(0, std::ios::beg);

        return endpos - begpos;
    }


    std::optional<TextFile> ReadText(std::string_view path) {
        if (!exists(path)) return std::nullopt;

        std::ifstream stream(path.data());
        if (stream.bad()) return std::nullopt;

        TextFile result;
        std::string tmp;

        while (std::getline(stream, tmp)) result.push_back(tmp);

        return stream.bad() ? std::nullopt : std::optional{ result };
    }


    bool WriteText(std::string_view path, const TextFile& text) {
        std::ofstream stream(path.data());
        if (stream.bad()) return false;

        for (const std::string& str : text) stream << str << '\n';

        return !stream.bad();
    }


    std::optional<DataFile> ReadData(std::string_view path) {
        if (!exists(path)) return std::nullopt;

        std::ifstream stream(path.data(), std::ios::binary);
        if (stream.bad()) return std::nullopt;

        std::size_t size = file_size(stream);

        DataFile result;
        result.resize(size);

        stream.read(reinterpret_cast<char*>(&result[0]), size);

        return stream.bad() ? std::nullopt : std::optional{ result };
    }


    bool WriteData(std::string_view path, const DataFile& data) {
        std::ofstream stream(path.data(), std::ios::binary);
        if (stream.bad()) return false;

        stream.write(reinterpret_cast<const char*>(&data[0]), data.size());

        return stream.bad();
    }
}