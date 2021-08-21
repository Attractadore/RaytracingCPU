cmake_minimum_required(VERSION 3.0)
project("Raytracing")

include("cmake/CMakeTemplates/PlatformFlags.cmake")
include("cmake/CMakeTemplates/CXXFlags.cmake")

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory("src")
