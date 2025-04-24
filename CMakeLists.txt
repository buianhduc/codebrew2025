cmake_minimum_required(VERSION 3.28)
project(codebrew C)

set(CMAKE_C_STANDARD 11)

include_directories(lib)
include_directories(src)

add_executable(codebrew
        lib/tigr.c
        lib/tigr.h
        src/cube.c
        src/cube.h
        src/main.c
        src/ray.c
        src/ray.h
        src/triangles.c
        src/triangles.h)

target_link_libraries(codebrew "-framework OpenGL" "-framework Cocoa")
