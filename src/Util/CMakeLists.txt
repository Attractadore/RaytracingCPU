add_library(
    DynamicLoaderLib
    "DynamicLoader.cpp"
)
target_link_libraries(
    DynamicLoaderLib
    PRIVATE Boost::boost
            platform_dynamic_linker
)

add_library(
    ReinhardToneMapping INTERFACE
    "ReinhardToneMapping.hpp"
)
target_link_libraries(
    ReinhardToneMapping
    INTERFACE cxx_parallel_algorithms
)

add_library(
    SDLHandleLib
    "SDLHandle.cpp"
)
target_link_libraries(
    SDLHandleLib
    PUBLIC SDL2::SDL2
)

add_library(
    WindowLib
    "Window.cpp"
)
target_link_libraries(
    WindowLib
    PUBLIC glm::glm
    PRIVATE SDLHandleLib
)

add_library(
    UtilLib SHARED
    "Lighting.cpp" "Math.hpp" "Random.cpp"
)
target_link_libraries(
    UtilLib
    PUBLIC glm::glm
           DynamicLoaderLib
           WindowLib
)
