get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# only include final code
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE aoc_common)
target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/input.txt")
    configure_file(input.txt input.txt COPYONLY)
endif()
