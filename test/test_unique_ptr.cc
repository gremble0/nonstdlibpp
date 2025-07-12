#include "unique_ptr.hh"

#include <catch2/catch_test_macros.hpp>

struct complex_type {
    int x;
    int y;
    void *v;
};

TEST_CASE("Test make_unique") {
    SECTION("Test basic usage") {
        auto heap_int = nstd::make_unique<int>(2);
        REQUIRE(*heap_int.get() == 2);
    }

    SECTION("Test for complex types") {
        auto heap_complex_type = nstd::make_unique<complex_type>(1, 2, nullptr);
        REQUIRE(heap_complex_type->x == 1);
        REQUIRE(heap_complex_type->y == 2);
        REQUIRE(heap_complex_type->v == nullptr);
    }
}

TEST_CASE("Test unique_ptr rule of 5") {
    SECTION("Test destructor") {}

    // Copy constructor and copy assignment operator are deleted

    SECTION("Test move constructor") {
        auto heap_int = nstd::make_unique<int>(2);
        nstd::unique_ptr<int> moved(std::move(heap_int));
        REQUIRE(heap_int.get() == nullptr);
        REQUIRE(*moved.get() == 2);
    }

    SECTION("Test move assignment operator") {
        auto heap_int = nstd::make_unique<int>(2);
        nstd::unique_ptr<int> moved = std::move(heap_int);
        REQUIRE(heap_int.get() == nullptr);
        REQUIRE(*moved.get() == 2);
    }
}
