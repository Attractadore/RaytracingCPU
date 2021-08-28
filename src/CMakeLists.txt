add_subdirectory("Util")

add_library(
    SceneLib SHARED
    "Intersectable.cpp" "Intersection.cpp" "Transform.cpp" "Ray.cpp"
    "Scene.cpp" "Camera.cpp" "Material.cpp" "Light.cpp"
)
target_link_libraries(
    SceneLib
    PUBLIC UtilLib
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

add_library(
    SceneLoaderLib
    "SceneLoader.cpp"
)
target_link_libraries(
    SceneLoaderLib
    PUBLIC SceneLib
    PRIVATE MaterialLoaderLib
)

add_executable(
    Raytracing
    "main.cpp"
)
target_link_libraries(
    Raytracing
    PRIVATE SceneLoaderLib
            Boost::program_options
            cxx_parallel_algorithms
)

add_subdirectory("Materials")
