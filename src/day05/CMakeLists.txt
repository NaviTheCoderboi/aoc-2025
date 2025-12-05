get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE aoc_common)
target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_options(${TARGET_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wshadow
    -Wundef
    -Wfloat-equal
)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

target_compile_options(${TARGET_NAME} PRIVATE
    -O3
    -march=native
    -mtune=native
    -funroll-loops
    -fomit-frame-pointer
    -fno-exceptions
    -fno-rtti
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/input.txt")
    configure_file(input.txt input.txt COPYONLY)
endif()
