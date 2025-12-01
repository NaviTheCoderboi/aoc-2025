cmake_minimum_required(VERSION 3.20)

project(aoc)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR src)
set(INCLUDE_DIR include)

file(GLOB_RECURSE COMMON_SRC ${SRC_DIR}/common/*.cpp)
add_library(aoc_common ${COMMON_SRC})
target_include_directories(aoc_common PUBLIC ${INCLUDE_DIR})

file(GLOB DAY_DIRS ${SRC_DIR}/day*)

foreach(day ${DAY_DIRS})
    add_subdirectory(${day})
endforeach()
