find_package(Boost COMPONENTS headers REQUIRED)
find_platform_dynamic_linker()
if (NOT TARGET platform_dynamic_linker)
    message(FATAL_ERROR "Dynamic linker required!")
endif()
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
    UtilLib SHARED
    "Lighting.cpp" "Math.cpp" "Color.cpp" "Random.cpp"
)
target_link_libraries(
    UtilLib
    PUBLIC glm::glm
           DynamicLoaderLib
)
