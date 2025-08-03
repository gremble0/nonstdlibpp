#include "string.hh"

#include <catch2/catch_test_macros.hpp>
#include <string>
TEST_CASE("Test string constructors") {
    SECTION("Default constructor") {
        nstd::string s;
        std::string gg;
        gg.size();
        REQUIRE(s.data() == nullptr);
        REQUIRE(s.size() == 0);
        //
    }

    SECTION("Size with char") {
        //
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
