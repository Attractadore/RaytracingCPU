#include "Material.hpp"
#include "MaterialInterface.hpp"
#include "Ray.hpp"
#include "Scene.hpp"

#include <glm/geometric.hpp>

struct Mirror: public Material {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene) const override;
};

glm::vec3 Mirror::evaluate(MaterialInput input, const Scene& scene) const {
    glm::vec3 incident = input.incident;
    glm::vec3 position = input.position;
    glm::vec3 normal = input.normal;

    glm::vec3 reflect = glm::reflect(incident, normal);

    return sceneIntersectColor(scene, Ray{position, reflect});
}

std::unique_ptr<Material> makeMaterial() {
    return std::make_unique<Mirror>();
}
