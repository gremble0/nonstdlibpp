#include <vector.hh>

#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("Test vector constructors") {
    SECTION("Test default constructor") {
        nstd::vector<int> vec;
        REQUIRE(vec.empty());
        REQUIRE(vec.size() == 0U);
        REQUIRE(vec.capacity() == 8U);
        REQUIRE(vec.data() == nullptr);
    }

    SECTION("Test initializer list constructor") {
        nstd::vector vec{1, 2, 3};
        REQUIRE(!vec.empty());
        REQUIRE(vec.size() == 3U);
        REQUIRE(vec.capacity() == 3U);
        // There are accessor/iterator tests for checking each value gets initialized correctly
        REQUIRE(vec.data() != nullptr);
    }

    SECTION("Test size constructor") {
        nstd::vector<int> vec(10U);
        REQUIRE(vec.size() == 0U);
        REQUIRE(vec.capacity() == 10U);
        REQUIRE(vec.data() == nullptr);
    }
}

TEST_CASE("Test vector iterators") {
    nstd::vector vec{1, 2, 3};

    SECTION("Test iterating with for each") {
        int i = 1;
        for (const auto &element : vec) {
            REQUIRE(element == i++);
        }
    }

    SECTION("Test iterating with iterators") {
        int i = 1;
        // NOLINTNEXTLINE(modernize-loop-convert)
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            REQUIRE(*it == i++);
        }
    }

    SECTION("Test pointer arithmetic on iterators") {
        REQUIRE(*vec.begin() == 1);
        REQUIRE(*(vec.begin() + 1) == 2);
        REQUIRE(*(vec.begin() + 2) == 3);
        REQUIRE(*(vec.end() - 1) == 3);
        REQUIRE(*(vec.end() - 2) == 2);
        REQUIRE(*(vec.end() - 3) == 1);
    }
}

TEST_CASE("Test vector accessors") {
    nstd::vector vec{1, 2, 3};
    SECTION("Test vector::operator[]") {
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

TEST_CASE("Test vector allocations") {
    SECTION("Test vector::reserve") {
        struct complex_type {
            int x;
            nstd::vector<int> values;
        };

        complex_type complex_variable1{.x = 2, .values = {1, 2, 3}};
        complex_type complex_variable2{.x = 69, .values = {50, 42, 123}};

        nstd::vector<complex_type> vec{complex_variable1, complex_variable2};
        std::vector<int> a{1, 2};
        REQUIRE(vec.capacity() == 20U);
        // Check old items are still there
    }
}
