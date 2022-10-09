#include <doctest/doctest.h>

#include "LineView.hpp"

#include <sstream>
#include <string>
#include <vector>

namespace doctest {
template <>
struct StringMaker<std::vector<std::string>> {
    static String convert(const std::vector<std::string> &vec) {
        std::ostringstream oss;

        oss << "{\n";
        for (const std::string &str : vec) {
            oss << "\"" << str << "\"" << std::endl;
        }
        oss << "}";

        return oss.str().c_str();
    }
};
} // namespace doctest

TEST_CASE("Trivial LineView") {
    const char *str = "1\n2\n3\n\n5";
    std::vector<std::string> expected{"1", "2", "3", "", "5"};
    std::vector<std::string> lines;
    for (auto &&line : LineView(std::string_view(str))) {
        lines.emplace_back(line);
    }
    CHECK_EQ(expected, lines);
}

TEST_CASE("LineView of temporary string") {
    auto stringProducer = []() -> std::string { return "Hello, world!\nThis is second line."; };
    std::vector<std::string> expected{"Hello, world!", "This is second line."};
    std::vector<std::string> lines;
    for (auto &&line : LineView(stringProducer())) {
        lines.emplace_back(line);
    }
    CHECK_EQ(expected, lines);
}
