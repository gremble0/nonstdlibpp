set(TESTS
  test_vector.cc
  test_unique_ptr.cc
)

find_package(Catch2 3 REQUIRED)
add_executable(test ${TESTS})
target_link_libraries(test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test PRIVATE ${PROJECT})
