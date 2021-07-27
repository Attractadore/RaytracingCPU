#include "Material.hpp"
#include "MaterialInterface.hpp"
#include "Ray.hpp"
#include "Scene.hpp"

#include <glm/geometric.hpp>

struct Mirror: public Material {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene, glm::vec3 eye_position) override;
};

glm::vec3 Mirror::evaluate(MaterialInput input, const Scene& scene, glm::vec3 eye_position) {
    glm::vec3 position = input.position;
    glm::vec3 normal = input.normal;

    float eye_dist = glm::length(position - eye_position);
    glm::vec3 from_camera_dir = (position - eye_position) / eye_dist;
    glm::vec3 reflect = glm::reflect(from_camera_dir, normal);

    glm::vec3 reflected_eye_position = position - eye_dist * reflect;

    return sceneIntersectColor(scene, Ray{position, reflect}, reflected_eye_position);
}

std::unique_ptr<Material> makeMaterial() {
    return std::make_unique<Mirror>();
}
