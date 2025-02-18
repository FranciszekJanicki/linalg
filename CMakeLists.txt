cmake_minimum_required(VERSION 3.28)

if(NOT DEFINED MAKE_PROJECT_NAME)
    set(MAKE_PROJECT_NAME linalg)
endif()

project(${MAKE_PROJECT_NAME})

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_STANDARD 23)

add_library(lib STATIC
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
    "lib/transforms.hpp"
    "lib/signals.hpp"
    "lib/utility.hpp")
target_compile_options(lib PUBLIC
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
    -Wcast-align)
set_target_properties(lib PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(lib PUBLIC "lib")


add_executable(polynomial "test/polynomial.cpp")
target_link_libraries(polynomial PRIVATE lib)

add_executable(regulators "test/regulators.cpp")
target_link_libraries(regulators PRIVATE lib)

add_executable(transforms "test/transforms.cpp")
target_link_libraries(transforms PRIVATE lib)

add_executable(filters "test/filters.cpp")
target_link_libraries(filters PRIVATE lib)

add_executable(signals "test/signals.cpp")
target_link_libraries(signals PRIVATE lib)