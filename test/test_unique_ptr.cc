#include "unique_ptr.hh"

#include <catch2/catch_test_macros.hpp>
#include <memory>

struct complex_type {
    int x;
    int y;
    void *v;
};

TEST_CASE("Test unique_ptr size is same as STL") {
    STATIC_REQUIRE(sizeof(std::unique_ptr<int>) == sizeof(nstd::unique_ptr<int>));
}

TEST_CASE("Test make_unique") {
    SECTION("Test basic usage") {
        auto heap_int = nstd::make_unique<int>(2);
        REQUIRE(*heap_int == 2);
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
        REQUIRE(*moved == 2);
    }

    SECTION("Test move assignment operator") {
        auto heap_int = nstd::make_unique<int>(2);
        nstd::unique_ptr<int> moved = std::move(heap_int);
        REQUIRE(heap_int.get() == nullptr);
        REQUIRE(*moved == 2);
    }
}

TEST_CASE("Test unique_ptr operator==") {
    SECTION("Same value") {
        auto heap_int1 = nstd::make_unique<int>(2);
        auto heap_int2 = nstd::make_unique<int>(2);
        REQUIRE(heap_int1 != heap_int2);
    }

    SECTION("Different value") {
        auto heap_int1 = nstd::make_unique<int>(1);
        auto heap_int2 = nstd::make_unique<int>(2);
        REQUIRE(heap_int1 != heap_int2);
    }

    SECTION("Same pointer") {
        auto heap_int1 = nstd::make_unique<int>(1);
        REQUIRE(heap_int1 == heap_int1);
    }

    SECTION("Default constructed unique_ptr with nullptr literal") {
        nstd::unique_ptr<int> a;
        REQUIRE(a == nullptr);
        REQUIRE(nullptr == a);
    }

    SECTION("Related by inheritance") {
        struct base {
            int a;
        };

        struct child : base {};

        auto heap_child1 = nstd::make_unique<child>();
        auto heap_child2 = nstd::make_unique<base>();
        REQUIRE(heap_child1 != heap_child2);
    }
}
