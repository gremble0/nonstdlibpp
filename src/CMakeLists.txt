set(SOURCES
  vector.cc
)

add_library(${PROJECT} ${SOURCES})
target_include_directories(${PROJECT} PUBLIC inc)
