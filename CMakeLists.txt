cmake_minimum_required(VERSION 2.8)

project(lz4xx)

include_directories(include)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE INCLUDES include/*.h)

add_library(lz4xx ${SRC})

file(GLOB_RECURSE EXAMPLE_SRC examples/*.cpp)

add_executable(lz4xx_example ${EXAMPLE_SRC})
target_link_libraries(lz4xx_example lz4xx)
