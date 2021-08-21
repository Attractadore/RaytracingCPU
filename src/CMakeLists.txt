add_subdirectory("Util")

add_library(
    SceneLib SHARED
    "Intersectable.cpp" "Intersection.cpp" "Transform.cpp" "Ray.cpp"
    "Scene.cpp" "Camera.cpp" "Material.cpp" "Light.cpp"
)
target_link_libraries(
    SceneLib
    PUBLIC glm::glm
    PRIVATE UtilLib
)
target_include_directories(
    SceneLib
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(
    MaterialLoaderLib
    "MaterialLoader.cpp"
)
target_link_libraries(
    MaterialLoaderLib
    PRIVATE UtilLib
)

find_cxx_parallel_algorithms()
if (NOT TARGET cxx_parallel_algorithms)
    message(FATAL_ERROR "C++ parallel algorithms required!")
endif()
add_executable(
    Raytracing
    "main.cpp"
)
target_link_libraries(
    Raytracing
    PRIVATE SDL2::SDL2
            cxx_parallel_algorithms
            SceneLib MaterialLoaderLib
)

add_subdirectory("Materials")
