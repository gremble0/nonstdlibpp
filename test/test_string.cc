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

    SECTION("Copy constructor") {
        //
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
