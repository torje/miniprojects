cmake_minimum_required(VERSION 4.2)

project(lambda)

set(CMAKE_VERBOSE_MAKEFILE on)

add_executable( lambda )

target_include_directories( lambda  PRIVATE . )
target_sources(lambda 
  PRIVATE
  source/main.c
  PUBLIC
)


