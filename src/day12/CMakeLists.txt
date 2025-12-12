get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# only include final code
add_executable(${TARGET_NAME} main.cpp)
target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_options(${TARGET_NAME} PRIVATE
        -O3
        -march=native
        -mtune=native
        -ffast-math
        -fno-math-errno
        -funroll-loops
        -fomit-frame-pointer
        -fstrict-aliasing
        -fno-exceptions
        -fno-rtti
        -flto
        -fvisibility=hidden
    )
target_link_options(${TARGET_NAME} PRIVATE -flto)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/input.txt")
    configure_file(input.txt input.txt COPYONLY)
endif()
