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
