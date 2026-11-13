cmake_minimum_required(VERSION 4.2)

project(storage)

set(CMAKE_VERBOSE_MAKEFILE on)

add_executable( test_array )

target_include_directories( test_array PRIVATE . )
target_link_libraries(test_array PRIVATE asan ubsan cunit)
target_sources(test_array
  PRIVATE
  unittest/test_array.c
  array/array.c
  PUBLIC
)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"   OR
    CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
    target_compile_options(test_array
      PRIVATE
      "-fsanitize=undefined"
      "-fsanitize=address"
    )
endif()


