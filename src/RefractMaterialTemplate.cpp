#pragma once
#include "Intersectable.hpp"
#include "Material.hpp"
#include "MaterialInterface.hpp"
#include "Scene.hpp"

#include <glm/geometric.hpp>

template <
    float refractive_index>
struct RefractiveMaterialTemplate: public Material {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene) const override;
};

template <
    float refractive_index>
glm::vec3 RefractiveMaterialTemplate<refractive_index>::evaluate(MaterialInput input, const Scene& scene) const {
    glm::vec3 direction = glm::refract(input.incident, input.normal, 1.0f / refractive_index);
    if (direction == glm::vec3{0.0f}) {
        direction = glm::reflect(input.incident, input.normal);
        return sceneIntersectColor(scene, Ray{input.position, direction});
    }

    while (true) {
        auto new_intersection = input.object->intersect(rayAdvanced(Ray{input.position, direction}, scene.eps));
        assert(new_intersection);

        input = new_intersection.input;
        direction = glm::refract(input.incident, -input.normal, refractive_index);
        if (direction != glm::vec3{0.0f}) {
            break;
        }
        direction = glm::reflect(input.incident, -input.normal);
    }

    return sceneIntersectColor(scene, Ray{input.position, direction});
}
