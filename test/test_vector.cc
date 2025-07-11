#include <initializer_list>
#include <utility>
#include <vector.hh>

#include <catch2/catch_test_macros.hpp>
#include <vector>

// Used in some tests to ensure vectors construct/destruct/move/copy, etc. complex types properly
struct complex_type {
    int x;
    nstd::vector<int> values;

    constexpr bool operator==(const complex_type &other) const { return x == other.x && values == other.values; }
};

TEST_CASE("Test vector size is same as STL") { STATIC_REQUIRE(sizeof(std::vector<int>) == sizeof(nstd::vector<int>)); }

TEST_CASE("Test vector constructors") {
    SECTION("Test default constructor") {
        nstd::vector<int> vec;
        REQUIRE(vec.empty());
        REQUIRE(vec.size() == 0U);
        REQUIRE(vec.capacity() == 0U);
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

    SECTION("Test vector::front") {
        REQUIRE(vec.front() == 1);
        REQUIRE(std::as_const(vec).front() == 1);
    }

    SECTION("Test vector::back") {
        REQUIRE(vec.back() == 3);
        REQUIRE(std::as_const(vec).back() == 3);
    }
}

TEST_CASE("Test equality operator") {
    complex_type complex_variable1{.x = 2, .values = {1, 2, 3}};
    complex_type complex_variable2{.x = 69, .values = {50, 42, 123}};

    nstd::vector vec1{complex_variable1, complex_variable2};
    nstd::vector vec2{complex_variable1, complex_variable2};
    nstd::vector vec3{complex_variable2, complex_variable1};

    REQUIRE(vec1 == vec2);
    REQUIRE(vec1 != vec3);

    REQUIRE(vec2 == vec1);
    REQUIRE(vec2 != vec3);

    REQUIRE(vec3 != vec1);
    REQUIRE(vec3 != vec2);
}

TEST_CASE("Test vector allocations") {
    SECTION("Test vector::reserve") {
        complex_type complex_variable1{.x = 2, .values = {1, 2, 3}};
        complex_type complex_variable2{.x = 69, .values = {50, 42, 123}};

        nstd::vector vec{complex_variable1, complex_variable2};
        vec.reserve(20U);
        REQUIRE(vec.capacity() == 20U);
        // Check old items are still there after reallocation
        REQUIRE(vec[0].x == complex_variable1.x);
        REQUIRE(vec[1].x == complex_variable2.x);
        REQUIRE(vec[0] == complex_variable1);
        REQUIRE(vec[1] == complex_variable2);
    }

    SECTION("Test vector::clear") {
        nstd::vector vec{1, 2, 3};
        // Calling clear should call the destructor for each element in the vector and set the size to 0, but it should
        // not do any dealloaction or allocation. The internal data should still point to the same memory after calling
        // clear()
        auto *data = vec.data();
        REQUIRE(data != nullptr);
        REQUIRE(vec.size() == 3);
        REQUIRE(vec.capacity() == 3);

        vec.clear();
        REQUIRE(vec.data() == data);
        REQUIRE(vec.size() == 0);
    }
}

TEST_CASE("Test push and pop") {
    SECTION("Test vector::push_back") {
        nstd::vector vec{1, 2, 3};
        vec.push_back(2);
        REQUIRE(vec.size() == 4);
        REQUIRE(vec[3] == 2);
    }

    SECTION("Test vector::emplace_back") {
        nstd::vector vec{1, 2, 3};
        vec.emplace_back(2);
        REQUIRE(vec.size() == 4);
        REQUIRE(vec[3] == 2);
    }

    SECTION("Test vector::emplace_back constructs in place") {
        // std::vector also seems to be unable to deduce the type of the second parameter unless we explicitly name the
        // constructor. e.g.:
        // vec.emplace_back(2, {1, 2, 3});
        //                     ^ it cannot deduce the type of this initializer_list.
        // Our implementation has the same issue, but since the STL also has the same, I will not look into it.
        nstd::vector<complex_type> vec;
        vec.emplace_back(2, nstd::vector{1, 2, 3});
        REQUIRE(vec[0].x == 2);
        REQUIRE(vec[0].values == nstd::vector{1, 2, 3});
    }
}
