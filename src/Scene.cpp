#include "Scene.hpp"
#include "Intersectable.hpp"
#include "Material.hpp"
#include "Util/Math.hpp"

#include <algorithm>

bool sceneHasIntersection(const Scene& scene, Ray r) {
    return std::find_if(
        scene.models.begin(), scene.models.end(),
        [&](const Model& model) {
            return model.hasIntersection(r, scene.eps);
        }
    ) != scene.models.end();
}

ModelIntersection sceneIntersect(const Scene& scene, Ray r) {
    ModelIntersection nearest_intersection;
    for (const auto& model: scene.models) {
        auto intersection = model.intersect(r);
        if (intersection and inInterval(scene.eps, nearest_intersection.t, intersection.t)) {
            nearest_intersection = intersection;
        }
    }
    return nearest_intersection;
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
