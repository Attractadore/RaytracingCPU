#include "SingleColorMCMaterialTemplate.cpp"

using Grey = SingleColorNonMetallicMCMaterial<glm::vec3{0.5f}, 0.9f, 1.46f>;

const Material* getMaterial() {
    return getMaterialSubclass<Grey>();
}
