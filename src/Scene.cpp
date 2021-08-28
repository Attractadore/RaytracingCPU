#include "Scene.hpp"
#include "Intersectable.hpp"
#include "Material.hpp"
#include "Util/Math.hpp"

bool sceneHasIntersection(const Scene& scene, Ray r) {
    for (const auto& model: scene.models) {
        if (model.hasIntersection(r, scene.eps)) {
            return true;
        }
    }
    return false;
}

ModelIntersection sceneIntersect(const Scene& scene, Ray r) {
    ModelIntersection nearest;
    for (const auto& model: scene.models) {
        auto intersection = model.intersect(r);
        if (intersection and inInterval(scene.eps, nearest.t, intersection.t)) {
            nearest = intersection;
        }
    }
    return nearest;
}

glm::vec3 sceneIntersectColor(const Scene& scene, Ray r) {
    auto intersection = sceneIntersect(scene, r);

    if (!intersection) {
        return glm::vec3{0.0f};
    }

    MaterialInput input = {
        .model = intersection.model,
        .view = -r.direction,
        .position = intersection.position,
        .normal = intersection.normal,
        .uv = intersection.uv,
        .bounces = r.bounces,
    };

    return intersection.model->material->evaluate(input, scene);
}
