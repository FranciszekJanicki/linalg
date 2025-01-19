cmake_minimum_required(VERSION 3.28)

if(NOT DEFINED MAKE_PROJECT_NAME)
    set(MAKE_PROJECT_NAME linalg)
endif()

project(${MAKE_PROJECT_NAME})

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_STANDARD 23)

add_library(Lib STATIC
    "lib/kalman.hpp"
    "lib/heap_matrix.hpp"
    "lib/vector3d.hpp"
    "lib/rotation3d.hpp"
    "lib/quaternion3d.hpp"
    "lib/dijkstra.hpp"
    "lib/heap_vector.hpp"
    "lib/pid.hpp"
    "lib/lqr_finite.hpp"
    "lib/lqr_infinite.hpp"
    "lib/observer.hpp"
    "lib/adrc.hpp"
    "lib/binary.hpp"
    "lib/ternary.hpp"
    "lib/stack_vector.hpp"
    "lib/stack_matrix.hpp"
    "lib/polynomial.hpp"
)

set_target_properties(Lib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Lib PUBLIC
    "lib"
)

find_package(fmt REQUIRED)
target_link_libraries(Lib PUBLIC
    fmt::fmt
)

add_executable(Kalman
    "app/kalman_test.cpp"
)

target_link_libraries(Kalman PRIVATE
    Lib
)

add_executable(Polynomial
    "app/polynomial_test.cpp"
)

target_link_libraries(Polynomial PRIVATE
    Lib
)

target_compile_options(Lib PUBLIC
    -O3
    -std=c++23
    -fconcepts
    -Wall
    -Wextra
    -Wconversion
    -Wshadow
    -Wpedantic
    -Wnarrowing
    -Waddress
    -Wnon-virtual-dtor
    -pedantic
    -Wpessimizing-move
    -Wsign-conversion
    -Wmisleading-indentation
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wnull-dereference
    -Wuseless-cast
    -Wdouble-promotion
    -Wformat=2
    -Wimplicit-fallthrough
    -Wcast-align
)
