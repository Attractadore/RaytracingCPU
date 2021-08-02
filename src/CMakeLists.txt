add_library(RayTracingLib SHARED
               "Camera.cpp"
               "Intersection.cpp"
               "Intersectable.cpp"
               "Light.cpp"
               "Material.cpp"
               "MaterialFactory.cpp"
               "Ray.cpp"
               "Scene.cpp"
               "Transform.cpp"
               "Util/Lighting.cpp"
               "Util/Math.cpp"
               "Util/Color.cpp"
)
target_link_libraries(RayTracingLib PUBLIC glm::glm PRIVATE dl)

add_executable(RayTracing "main.cpp")
target_link_libraries(RayTracing PRIVATE SDL2::SDL2 RayTracingLib)

add_library(MarbleMaterial SHARED "MarbleMaterial.cpp")
target_link_libraries(MarbleMaterial PRIVATE RayTracingLib)

add_library(BlueMaterial SHARED "BlueMaterial.cpp")
target_link_libraries(BlueMaterial PRIVATE RayTracingLib)

add_library(GreyMaterial SHARED "GreyMaterial.cpp")
target_link_libraries(GreyMaterial PRIVATE RayTracingLib)

add_library(GoldMaterial SHARED "GoldMaterial.cpp")
target_link_libraries(GoldMaterial PRIVATE RayTracingLib)

add_library(MirrorMaterial SHARED "MirrorMaterial.cpp")
target_link_libraries(MirrorMaterial PRIVATE RayTracingLib)

add_library(GlassMaterial SHARED "GlassMaterial.cpp")
target_link_libraries(GlassMaterial PRIVATE RayTracingLib)

add_library(RealGlassMaterial SHARED "RealGlassMaterial.cpp")
target_link_libraries(RealGlassMaterial PRIVATE RayTracingLib)

add_library(LessDenseMaterial SHARED "LessDenseMaterial.cpp")
target_link_libraries(LessDenseMaterial PRIVATE RayTracingLib)
