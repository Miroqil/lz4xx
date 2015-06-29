cmake_minimum_required(VERSION 2.8)

project(lz4xx)

include_directories(include)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE INCLUDES include/*.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")

add_library(lz4xx ${SRC})

set(SRC_EX1 "examples/example1.cpp")
add_executable(lz4xx_example1 ${SRC_EX1})
target_link_libraries(lz4xx_example1 lz4xx)

set(SRC_EX2 "examples/example2.cpp")
add_executable(lz4xx_example2 ${SRC_EX2})
target_link_libraries(lz4xx_example2 lz4xx)
