cmake_minimum_required(VERSION 3.0)
project("Raytracing")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

include("cmake/CMakeTemplates/PlatformFlags.cmake")
include("cmake/CMakeTemplates/CXXFlags.cmake")

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(Boost COMPONENTS headers program_options REQUIRED)
find_platform_dynamic_linker()
if (NOT TARGET platform_dynamic_linker)
    message(FATAL_ERROR "Dynamic linker required!")
endif()
find_cxx_parallel_algorithms()
if (NOT TARGET cxx_parallel_algorithms)
    message(FATAL_ERROR "C++ parallel algorithms required!")
endif()

add_subdirectory("src")
