cmake_minimum_required(VERSION 3.0)
project("Raytracing")

set(CMAKE_BUILD_TYPE "Release")

include("cmake/CMakeTemplates/PlatformFlags.cmake")
include("cmake/CMakeTemplates/CXXFlags.cmake")

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory("src")
