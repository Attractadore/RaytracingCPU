#include "SingleColorMCMaterialTemplate.cpp"

using Blue = SingleColorNonMetallicMCMaterial<glm::vec3{0.0f, 0.0f, 1.0f}, 0.7f, 1.46f>;

const Material* getMaterial() {
    return getMaterialSubclass<Blue>();
}
