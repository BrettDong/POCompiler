#include <algorithm>
#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string_view>

#include "LineView.hpp"
#include "MemMapFileReader.hpp"
#include "fmt/format.h"

bool starts_with(const std::string_view str, const std::string_view prefix) {
    if (str.length() < prefix.length()) {
        return false;
    }
    return str.substr(0, prefix.length()).compare(prefix) == 0;
}

bool ends_with(const std::string_view str, const std::string_view suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.substr(str.length() - suffix.length()).compare(suffix) == 0;
}

constexpr std::size_t numPluralForms = 6;

std::string unescape(std::string_view input) {
    static const std::map<char, char> chars = {
        {'n', '\n'}, {'r', '\r'}, {'t', '\t'}, {'\\', '\\'}, {'"', '"'}};
    std::string result;
    result.reserve(input.length());
    // for (std::size_t i = 0; i < input.length(); i++) {
    //     if (input[i] == '\\') {
    //         auto ch = chars.find(input[i + 1]);
    //         if (ch != chars.end()) {
    //             result.append(1, ch->second);
    //         } else {
    //             throw std::runtime_error(fmt::format("Unknown escape char \\{}", input[i + 1]));
    //         }
    //         i++;
    //     } else {
    //         result.append(1, input[i]);
    //     }
    // }
    // return result;
    std::size_t p = 0;
    while (true) {
        std::size_t pos = input.find('\\', p);
        if (pos != std::string_view::npos) {
            result.append(input.substr(p, pos - p));
            auto ch = chars.find(input[pos + 1]);
            if (ch != chars.end()) {
                result.append(1, ch->second);
            }
            p = pos + 2;
            if (p == input.length()) {
                break;
            }
        } else {
            result.append(input.substr(p));
            break;
        }
    }
    return result;
}

struct POEntry {
    std::string msgctxt;
    std::string msgid;
    std::string msgid_plural;
    std::string msgstr;
    std::array<std::string, numPluralForms> msgstr_plural;

    void clear() {
        msgctxt.clear();
        msgid.clear();
        msgid_plural.clear();
        msgstr.clear();
        for (std::string &str : msgstr_plural) {
            str.clear();
        }
    }

    bool isSingular() const {
        return msgid_plural.empty();
    }

    bool isPlural() const {
        return !msgid_plural.empty();
    }

    bool isTranslated() {
        if (isSingular()) {
            return !msgstr.empty();
        } else {
            return std::all_of(msgstr_plural.begin(), msgstr_plural.end(),
                               [](const std::string &str) { return !str.empty(); });
        }
    }

    std::string originalStringRepresentation() const {
        std::string result;
        if (!msgctxt.empty()) {
            result.append(unescape(msgctxt));
            result.append(1, '\x04');
        }
        result.append(unescape(msgid));
        if (isPlural()) {
            result.append(1, '\x00');
            result.append(unescape(msgid_plural));
        }
        return result;
    }

    std::string translatedStringRepresentation() const {
        std::string result;
        if (!isPlural()) {
            result.append(unescape(msgstr));
        } else {
            for (std::size_t i = 0; i < numPluralForms - 1; i++) {
                result.append(unescape(msgstr_plural[i]));
                result.append(1, '\x00');
            }
            result.append(unescape(msgstr_plural[numPluralForms - 1]));
        }
        return result;
    }
};

std::string_view extractStringContent(std::string_view line, std::size_t prefix_length) {
    return line.substr(prefix_length + 1, line.length() - prefix_length - 2);
}

std::map<std::string, std::string> parsePOFile(FileReader &file) {
    std::map<std::string, std::string> consolidatedEntries;
    POEntry currentEntry;
    std::string *currentStr = nullptr;
    for (auto &&line : LineView(file.view())) {
        if (line.empty()) {
            if (currentEntry.isTranslated()) {
                auto origRepr = currentEntry.originalStringRepresentation();
                auto translatedRepr = currentEntry.translatedStringRepresentation();
                consolidatedEntries[origRepr] = translatedRepr;
            }
            currentEntry.clear();
        } else if (starts_with(line, "#")) {
            continue;
        } else if (starts_with(line, "msgctxt ")) {
            currentStr = &currentEntry.msgctxt;
            currentStr->append(extractStringContent(line, std::strlen("msgctxt ")));
        } else if (starts_with(line, "msgid ")) {
            currentStr = &currentEntry.msgid;
            currentStr->append(extractStringContent(line, std::strlen("msgid ")));
        } else if (starts_with(line, "msgid_plural ")) {
            currentStr = &currentEntry.msgid_plural;
            currentStr->append(extractStringContent(line, std::strlen("msgid_plural ")));
        } else if (starts_with(line, "msgstr ")) {
            currentStr = &currentEntry.msgstr;
            currentStr->append(extractStringContent(line, std::strlen("msgstr ")));
        } else if (starts_with(line, "msgstr[0] ")) {
            currentStr = &currentEntry.msgstr_plural[0];
            currentStr->append(extractStringContent(line, std::strlen("msgstr[0] ")));
        } else if (starts_with(line, "msgstr[1] ")) {
            currentStr = &currentEntry.msgstr_plural[1];
            currentStr->append(extractStringContent(line, std::strlen("msgstr[1] ")));
        } else if (starts_with(line, "msgstr[2] ")) {
            currentStr = &currentEntry.msgstr_plural[2];
            currentStr->append(extractStringContent(line, std::strlen("msgstr[2] ")));
        } else if (starts_with(line, "msgstr[3] ")) {
            currentStr = &currentEntry.msgstr_plural[3];
            currentStr->append(extractStringContent(line, std::strlen("msgstr[3] ")));
        } else if (starts_with(line, "msgstr[4] ")) {
            currentStr = &currentEntry.msgstr_plural[4];
            currentStr->append(extractStringContent(line, std::strlen("msgstr[4] ")));
        } else if (starts_with(line, "msgstr[5] ")) {
            currentStr = &currentEntry.msgstr_plural[5];
            currentStr->append(extractStringContent(line, std::strlen("msgstr[5] ")));
        } else if (starts_with(line, "\"")) {
            currentStr->append(extractStringContent(line, 0));
        } else {
            throw std::runtime_error(fmt::format("Don't know how to deal with \"{}\"", line));
        }
    }
    return consolidatedEntries;
}

template <typename T, std::enable_if_t<std::is_integral_v<T>, void *> = nullptr>
void writeInteger(std::ofstream &fout, T val) {
    fout.write(reinterpret_cast<const char *>(&val), sizeof(T));
}

void writeMO(const std::map<std::string, std::string> &entries, const char *path) {
    std::ofstream fout(path, std::ios::out | std::ios::binary);
    writeInteger(fout, 0x950412deU);
    writeInteger(fout, 0x00000000U);
    std::uint32_t numEntries = entries.size();
    writeInteger(fout, numEntries);
    std::uint32_t OTableOffset = 28U;
    std::uint32_t TTableOffset = OTableOffset + 8 * numEntries;
    writeInteger(fout, OTableOffset);
    writeInteger(fout, TTableOffset);
    writeInteger(fout, 0x00000000U);
    writeInteger(fout, 0x00000000U);
    std::uint32_t offset = TTableOffset + 8 * numEntries;
    for (const auto &[O, T] : entries) {
        std::uint32_t OLength = O.length();
        writeInteger(fout, OLength);
        writeInteger(fout, offset);
        offset += OLength + 1;
    }
    for (const auto &[O, T] : entries) {
        std::uint32_t TLength = T.length();
        writeInteger(fout, TLength);
        writeInteger(fout, offset);
        offset += TLength + 1;
    }
    for (const auto &[O, T] : entries) {
        fout.write(O.c_str(), static_cast<std::uint32_t>(O.length()) + 1);
    }
    for (const auto &[O, T] : entries) {
        fout.write(T.c_str(), static_cast<std::uint32_t>(T.length()) + 1);
    }
    fout.close();
}

int main(int argc, char *argv[]) {
    std::string input_path;
    std::string output_path;
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_path = argv[++i];
        } else if (argv[i][0] == '-') {
            std::cout << "Warning: Unknown flag " << argv[i] << std::endl;
        } else if (ends_with(argv[i], ".po")) {
            input_path = argv[i];
        }
    }
    if (input_path.empty()) {
        std::cout << "Fatal: Input file path unspecified" << std::endl;
        return 1;
    }
    if (output_path.empty()) {
        std::cout << "Fatal: Output file path unspecified" << std::endl;
        return 1;
    }

    auto input = std::filesystem::path(input_path);
    auto output = std::filesystem::path(output_path);

    if (!std::filesystem::exists(input)) {
        std::cout << "Fatal: Input file \"" << input_path << "\" does not exist" << std::endl;
        return 1;
    }

    if (!std::filesystem::exists(output.parent_path())) {
        std::cout << "Fatal: Output path \"" << output.parent_path() << "\" does not exist"
                  << std::endl;
        return 1;
    }

    if (!input.has_extension() || input.extension() != ".po") {
        std::cout << "Fatal: Input file \"" << input_path << "\" does not seem to be a .po file"
                  << std::endl;
        return 1;
    }

    if (!output.has_extension() || output.extension() != ".mo") {
        std::cout << "Fatal: Output file \"" << input_path << "\" does not seem to be a .mo file"
                  << std::endl;
        return 1;
    }

    std::unique_ptr<FileReader> input_file = std::make_unique<MemMapFileReader>(input_path.c_str());
    auto entries = parsePOFile(*input_file);
    writeMO(entries, output_path.c_str());
    return 0;
}
