#include "shared_ptr.hh"

#include <catch2/catch_test_macros.hpp>
#include <memory>

struct complex_type {
    int x;
    int y;
    void *v;
};

TEST_CASE("Test shared_ptr size is same as STL") {
    STATIC_REQUIRE(sizeof(std::shared_ptr<int>) == sizeof(nstd::shared_ptr<int>));
}

TEST_CASE("Test make_shared") {
    SECTION("Test basic usage") {
        auto heap_int = nstd::make_shared<int>(2);
        REQUIRE(*heap_int == 2);
    }

    SECTION("Test for complex types") {
        auto heap_complex_type = nstd::make_shared<complex_type>(1, 2, nullptr);
        REQUIRE(heap_complex_type->x == 1);
        REQUIRE(heap_complex_type->y == 2);
        REQUIRE(heap_complex_type->v == nullptr);
    }
}

TEST_CASE("Test shared_ptr rule of 5") {
    SECTION("Test destructor") {}

    SECTION("Test copy constructor") {
        auto heap_int = nstd::make_shared<int>(2);
        // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
        auto copied(heap_int);
        REQUIRE(heap_int.use_count() == 2);
        REQUIRE(copied.use_count() == 2);

        // Ensure ref count gets decremented properly
        {
            // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
            auto another_copy(copied);
            REQUIRE(heap_int.use_count() == 3);
            REQUIRE(copied.use_count() == 3);
            REQUIRE(another_copy.use_count() == 3);
        }

        REQUIRE(heap_int.use_count() == 2);
        REQUIRE(copied.use_count() == 2);
    }

    // SECTION("Test move constructor") {
    //     auto heap_int = nstd::make_shared<int>(2);
    //     nstd::shared_ptr<int> moved(std::move(heap_int));
    //     REQUIRE(heap_int.get() == nullptr);
    //     REQUIRE(*moved == 2);
    // }
    //
    // SECTION("Test move assignment operator") {
    //     auto heap_int = nstd::make_shared<int>(2);
    //     nstd::shared_ptr<int> moved = std::move(heap_int);
    //     REQUIRE(heap_int.get() == nullptr);
    //     REQUIRE(*moved == 2);
    // }
}
