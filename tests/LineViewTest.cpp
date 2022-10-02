#include "doctest/doctest.h"

#include "LineView.hpp"

#include <string>
#include <vector>

TEST_CASE("LineView<std::string>") {
    using namespace std::string_literals;
    auto str = "1\n2\n3\n\n5"s;
    std::vector<std::string> expected{"1", "2", "3", "", "5"};
    std::vector<std::string> lines;
    for (auto &&line : LineView(str)) {
        lines.emplace_back(line);
    }
    CHECK_EQ(expected, lines);
}

TEST_CASE("LineView<std::string_view>") {
    const char *str = "1\n2\n3\n\n5";
    std::vector<std::string> expected{"1", "2", "3", "", "5"};
    std::vector<std::string> lines;
    for (auto &&line : LineView(std::string_view(str))) {
        lines.emplace_back(line);
    }
    CHECK_EQ(expected, lines);
}

TEST_CASE("LineView<std::vector<char>>") {
    std::vector<char> str{'1', '\n', '2', '\n', '3', '\n', '\n', '5'};
    std::vector<std::string> expected{"1", "2", "3", "", "5"};
    std::vector<std::string> lines;
    for (auto &&line : LineView(str)) {
        lines.emplace_back(line);
    }
    CHECK_EQ(expected, lines);
}
