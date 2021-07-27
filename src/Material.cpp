#include "Material.hpp"

#include "Scene.hpp"

glm::vec3 PerLightMaterial::evaluate(MaterialInput input, const Scene& scene, glm::vec3 eye_position) {
    glm::vec3 color{0.0f};
    for (auto light: scene.lights) {
        color += evaluate(input, scene, eye_position, light);
    }
    return color;
}
