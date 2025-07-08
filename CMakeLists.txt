cmake_minimum_required(VERSION 3.20)
set(PROJECT nonstdlibpp)
project(${PROJECT})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_subdirectory(src)
add_subdirectory(test)
