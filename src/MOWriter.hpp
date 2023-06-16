#pragma once

#include <cstdint>
#include <fstream>
#include <type_traits>

template <typename T>
requires std::is_integral_v<T>
void writeInteger(std::ofstream &fout, T val) {
    fout.write(reinterpret_cast<const char *>(&val), sizeof(T));
}

template <typename Map>
void writeMO(const Map &entries, const char *path) {
    std::ofstream fout(path, std::ios::out | std::ios::binary);
    writeInteger(fout, 0x950412deU);
    writeInteger(fout, 0x00000000U);
    auto numEntries = static_cast<std::uint32_t>(entries.size());
    writeInteger(fout, numEntries);
    std::uint32_t OTableOffset = 28U;
    std::uint32_t TTableOffset = OTableOffset + 8 * numEntries;
    writeInteger(fout, OTableOffset);
    writeInteger(fout, TTableOffset);
    writeInteger(fout, 0x00000000U);
    writeInteger(fout, 0x00000000U);
    std::uint32_t offset = TTableOffset + 8 * numEntries;
    constexpr std::uint32_t alignment = 4U;
    const char nil = '\0';
    for (const auto &[O, T] : entries) {
        auto OLength = static_cast<std::uint32_t>(O.length());
        writeInteger(fout, OLength);
        writeInteger(fout, offset);
        offset += OLength + 1;
        offset += (alignment - (OLength + 1) % alignment) % alignment;
    }
    for (const auto &[O, T] : entries) {
        auto TLength = static_cast<std::uint32_t>(T.length());
        writeInteger(fout, TLength);
        writeInteger(fout, offset);
        offset += TLength + 1;
        offset += (alignment - (TLength + 1) % alignment) % alignment;
    }
    for (const auto &[O, T] : entries) {
        fout.write(O.c_str(), static_cast<std::streamsize>(O.length()) + 1);
        auto rem = (O.length() + 1) % alignment;
        if (rem) {
            fout.write(&nil, static_cast<std::streamsize>(alignment - rem));
        }
    }
    for (const auto &[O, T] : entries) {
        fout.write(T.c_str(), static_cast<std::streamsize>(T.length()) + 1);
        auto rem = (T.length() + 1) % alignment;
        if (rem) {
            fout.write(&nil, static_cast<std::streamsize>(alignment - rem));
        }
    }
    fout.close();
}
