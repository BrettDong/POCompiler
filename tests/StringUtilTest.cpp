#include <doctest/doctest.h>

#include "StringUtil.hpp"

TEST_CASE("starts_with") {
    CHECK_EQ(starts_with("Hello, world!", "Hello"), true);
    CHECK_EQ(starts_with("Hello, world!", "hello"), false);
    CHECK_EQ(starts_with("", "hello"), false);
    CHECK_EQ(starts_with("hell", "hello"), false);
    CHECK_EQ(starts_with("hello", "hello"), true);
}

TEST_CASE("ends_with") {
    CHECK_EQ(ends_with("Hello, world!", "world!"), true);
    CHECK_EQ(ends_with("Hello, world!", "world"), false);
    CHECK_EQ(ends_with("", "world!"), false);
    CHECK_EQ(ends_with("orld!", "world!"), false);
    CHECK_EQ(ends_with("world!", "world!"), true);
}

TEST_CASE("join") {
    CHECK_EQ(join({}, '-'), "");
    CHECK_EQ(join({"1", "2"}, '-'), "1-2");
    CHECK_EQ(join({"-", "-"}, '-'), "---");
}

TEST_CASE("unescape") {
    const std::string raw = R"(1\n2)";
    const std::string expected = "1\n2";
    CHECK_EQ(unescape(raw), expected);
}
