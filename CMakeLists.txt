cmake_minimum_required(VERSION 3.20)

project(aoc)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR src)

file(GLOB DAY_DIRS ${SRC_DIR}/day*)

foreach(day ${DAY_DIRS})
    add_subdirectory(${day})
endforeach()
