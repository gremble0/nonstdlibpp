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
}
