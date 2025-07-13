#include "shared_ptr.hh"

#include <catch2/catch_test_macros.hpp>
#include <memory>

TEST_CASE("Test shared_ptr size is same as STL") {
    STATIC_REQUIRE(sizeof(std::shared_ptr<int>) == sizeof(nstd::shared_ptr<int>));
}

TEST_CASE("Test make_shared") {
    SECTION("Test basic usage") {
        auto heap_int = nstd::make_shared<int>(2);
        REQUIRE(*heap_int == 2);
    }

    SECTION("Test for complex types") {
        auto heap_complex_type = nstd::make_unique<complex_type>(1, 2, nullptr);
        REQUIRE(heap_complex_type->x == 1);
        REQUIRE(heap_complex_type->y == 2);
        REQUIRE(heap_complex_type->v == nullptr);
    }
}
