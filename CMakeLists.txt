cmake_minimum_required(VERSION 3.18)
project("RayTracingCPP")

include("cmake/CMakeTemplates/CFlags.cmake")
include("cmake/CMakeTemplates/CXXFlags.cmake")
include("cmake/CMakeTemplates/CopyToBuild.cmake")

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory("src")
