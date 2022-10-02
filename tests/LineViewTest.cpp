#include "doctest/doctest.h"

#include "LineView.hpp"

#include <vector>

TEST_CASE("LineView<std::string_view>") {
    const char *str = "1\n2\n3\n\n5";
    std::vector<std::string> expected{"1", "2", "3", "", "5"};
    std::vector<std::string> lines;
    for (auto &&line : LineView(std::string_view(str))) {
        lines.emplace_back(line);
    }
    CHECK_EQ(expected, lines);
}
