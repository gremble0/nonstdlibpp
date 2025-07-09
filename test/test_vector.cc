#include <vector.hh>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Test vector constructors") {
    SECTION("Test default constructor") {
        nstd::vector<int> vec;
        REQUIRE(vec.empty());
        REQUIRE(vec.size() == 0);
        REQUIRE(vec.capacity() == 8);
        REQUIRE(vec.data() == nullptr);
    }

    SECTION("Test initializer list constructor") {
        nstd::vector<int> vec{1, 2, 3};
        REQUIRE(!vec.empty());
        REQUIRE(vec.size() == 3);
        REQUIRE(vec.capacity() == 3);
        // There are accessor/iterator tests for checking each value gets initialized correctly
        REQUIRE(vec.data() != nullptr);
    }
}

TEST_CASE("Test vector iterators") {
    SECTION("Test iterating with for each") {
        nstd::vector<int> vec{1, 2, 3};
        int i = 1;
        for (const auto &element : vec) {
            REQUIRE(element == i++);
        }
    }

    SECTION("Test iterating with iterators") {
        nstd::vector<int> vec{1, 2, 3};
        int i = 1;
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            REQUIRE(*it == i++);
        }
    }

    SECTION("Test pointer arithmetic on iterators") {
        nstd::vector<int> vec{1, 2, 3};
        REQUIRE(*vec.begin() == 1);
        REQUIRE(*(vec.begin() + 1) == 2);
        REQUIRE(*(vec.begin() + 2) == 3);
        REQUIRE(*(vec.end() - 1) == 3);
        REQUIRE(*(vec.end() - 2) == 2);
        REQUIRE(*(vec.end() - 3) == 1);
    }
}

TEST_CASE("Test vector accessors") {
    nstd::vector<int> vec{1, 2, 3};
    SECTION("Test subscript operator") {
        REQUIRE(vec[0] == 1);
        REQUIRE(vec[1] == 2);
        REQUIRE(vec[2] == 3);
    }

    SECTION("Test vector::at") {
        REQUIRE(vec.at(0) == 1);
        REQUIRE(vec.at(1) == 2);
        REQUIRE(vec.at(2) == 3);
        REQUIRE_THROWS_AS(vec.at(3), std::out_of_range);
    }
}
