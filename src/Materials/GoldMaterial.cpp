#include "SingleColorMCMaterialTemplate.cpp"

using Gold = SingleColorMetallicMCMaterial<glm::vec3{1.0f, 0.782f, 0.344f}, 0.2f>;

Material* getMaterial() {
    return getMaterialSubclass<Gold>();
}