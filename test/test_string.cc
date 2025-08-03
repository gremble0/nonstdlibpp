#include "string.hh"

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <stdexcept>

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
        //
    }

    SECTION("Copy assignment operator") {
        //
    }

    SECTION("Move assignment operator") {
        //
    }
}

TEST_CASE("Test string iterator") {
    //
}

TEST_CASE("Trivial accessors") {
    SECTION("Test at()") {
        nstd::string s(10, 'h');
        REQUIRE_NOTHROW(s.at(9));
        REQUIRE_THROWS_AS(s.at(10), std::out_of_range);
    }
}

TEST_CASE("Test equality operators") {
    SECTION("const char *") {
        nstd::string s = "hello world";
        REQUIRE(s == "hello world");
    }
    SECTION("string") {
        nstd::string s = "hello world";
        REQUIRE(s == nstd::string("hello world"));
    }
}
