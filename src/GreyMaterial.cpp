#include "SingleColorBPMaterialTemplate.cpp"

using Grey = SingleColorBPMaterial<glm::vec3{0.5f}, 16.0f>;

std::unique_ptr<Material> makeMaterial() {
    return std::make_unique<Grey>();
}
