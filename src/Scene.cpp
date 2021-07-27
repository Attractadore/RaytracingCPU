#include "Scene.hpp"
#include "Intersectable.hpp"
#include "Material.hpp"

#include <cmath>

bool sceneHasIntersection(const Scene& scene, Ray r) {
    for (auto mo: scene.objects) {
        auto intersection_opt = mo->object->intersectDistance(r);
        if (intersection_opt && intersection_opt.value() > scene.eps) {
            return true;
        }
    }
    return false;
}

std::optional<MaterialIntersection> sceneIntersectMaterial(const Scene& scene, Ray r) {
    MaterialIntersection nearest_intersection;

    for (auto mo: scene.objects) {
        auto is_opt = mo->intersect(r);
        if (is_opt) {
            MaterialIntersection intersection = is_opt.value();
            if (intersection.t > scene.eps && intersection.t < nearest_intersection.t) {
                nearest_intersection = intersection;
            }
        }
    }

    if (std::isfinite(nearest_intersection.t)) {
        return nearest_intersection;
    }

    return {};
}

glm::vec3 sceneIntersectColor(const Scene& scene, Ray r) {
    return sceneIntersectColor(scene, r, scene.camera.position);
}

glm::vec3 sceneIntersectColor(const Scene& scene, Ray r, glm::vec3 eye_position) {
    auto intersection_opt = sceneIntersectMaterial(scene, r);
     
    if (!intersection_opt) {
        return glm::vec3{0.0f};
    }

    auto intersection = intersection_opt.value();

    return intersection.material->evaluate(intersection.input, scene, eye_position);
}
