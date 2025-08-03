#include "string.hh"

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <stdexcept>
#include <string>

TEST_CASE("Test string constructors") {
    SECTION("Default constructor") {
        nstd::string s;
        REQUIRE(s.data() == nullptr);
        REQUIRE(s.size() == 0);
    }

    SECTION("Size with char") {
        size_t s_size = 10;
        char s_char = 'h';
        nstd::string s(s_size, s_char);
        for (size_t i = 0; i < s_size; ++i) {
            REQUIRE(s[i] == s_char);
        }
    }

    SECTION("const char *") {
        const char *c_str = "hello";
        nstd::string s = c_str;
        REQUIRE(s.data() != nullptr);
        REQUIRE(s.data() != c_str);
    }

    SECTION("Copy constructor") {
        SECTION("const char *") {
            nstd::string a("abc");
            REQUIRE(a == "abc");
        }
        SECTION("string") {
            nstd::string a(nstd::string("abc"));
            REQUIRE(a == "abc");
        }
    }

    SECTION("Move constructor") {
        const char *cstr = "hello world";
        nstd::string s = cstr;
        nstd::string moved(std::move(s));
        REQUIRE(s.data() == nullptr);
        REQUIRE(moved.data() != nullptr);
        REQUIRE(s.size() == 0);
        REQUIRE(moved.size() != 0);
        REQUIRE(moved == cstr);
    }

    SECTION("Copy assignment operator") {
        nstd::string s("hello world");
        nstd::string copied;
        copied = s;
        REQUIRE(s.data() != copied.data());
        REQUIRE(s.size() == copied.size());
        REQUIRE(s == copied);
    }

    SECTION("Move assignment operator") {
        const char *cstr = "hello world";
        nstd::string s = cstr;
        nstd::string moved;
        moved = std::move(s);
        REQUIRE(s.data() == nullptr);
        REQUIRE(moved.data() != nullptr);
        REQUIRE(s.size() == 0);
        REQUIRE(moved.size() != 0);
        REQUIRE(moved == cstr);
    }
}

TEST_CASE("Test string iterator") {
    // Just compare with std as a source of truth
    const char *cstr = "hello world";
    nstd::string s1(cstr);
    std::string s2(cstr);
    auto it1 = s1.begin();
    auto it2 = s2.begin();
    for (; it1 != s1.end() && it2 != s2.end(); ++it1, ++it2) {
        REQUIRE(*it1 == *it2);
    }
}

TEST_CASE("Trivial accessors") {
    SECTION("Test at()") {
        nstd::string s(10, 'h');
        REQUIRE_NOTHROW(s.at(9));
        REQUIRE_THROWS_AS(s.at(10), std::out_of_range);
    }
}

TEST_CASE("Test equality operator") {
    nstd::string s = "hello world";
    REQUIRE(s == "hello world");
    REQUIRE(s != "hello worldd");
    REQUIRE(s != "hello worl");
}
