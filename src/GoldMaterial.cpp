#include "SingleColorMetallicCTMaterialTemplate.cpp"

using Gold = SingleColorMetallicCTMaterial<glm::vec3{1.0f, 0.782f, 0.344f}, 0.2f>;

std::unique_ptr<Material> makeMaterial() {
    return std::make_unique<Gold>();
}
