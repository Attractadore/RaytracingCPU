#include "SingleColorBPMaterialTemplate.cpp"

using Blue = SingleColorBPMaterial<glm::vec3{0.0f, 0.0f, 1.0f}, 32.0f>;

std::unique_ptr<Material> makeMaterial() {
    return std::make_unique<Blue>();
}
