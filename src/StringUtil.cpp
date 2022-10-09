#include "StringUtil.hpp"

#include <map>
#include <numeric>

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

std::string join(const std::vector<std::string> &lines, char delim) {
    if (lines.empty()) {
        return {};
    }
    std::size_t totalLength =
        std::accumulate(lines.begin(), lines.end(), lines.size(),
                        [](std::size_t l, const std::string &t) { return t.length() + l; });
    std::string result;
    result.reserve(totalLength);
    for (std::size_t i = 0; i + 1 < lines.size(); i++) {
        result.append(lines[i]);
        result.append(1, delim);
    }
    result.append(lines.back());
    return result;
}

std::string unescape(std::string_view input) {
    static const std::map<char, char> chars = {{'n', '\n'},  {'r', '\r'}, {'t', '\t'},
                                               {'\\', '\\'}, {'"', '"'},  {'b', '\b'}};
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
