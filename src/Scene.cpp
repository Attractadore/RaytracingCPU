#include "Scene.hpp"
#include "Intersectable.hpp"
#include "Material.hpp"
#include "Util/Math.hpp"

#include <cmath>

bool sceneHasIntersection(const Scene& scene, Ray r) {
    for (auto mo: scene.objects) {
        float t = mo->object->intersectDistance(r);
        assert(std::isfinite(t));
        if (t > scene.eps) {
            return true;
        }
    }

    return false;
}

Intersection sceneIntersect(const Scene& scene, Ray r) {
    Intersection nearest_intersection;

    for (auto mo: scene.objects) {
        auto intersection = mo->object->intersect(r);
        if (intersection && inInterval(scene.eps, nearest_intersection.t, intersection.t)) {
            nearest_intersection = intersection;
        }
    }

    return nearest_intersection;
}

MaterialIntersection sceneIntersectMaterial(const Scene& scene, Ray r) {
    MaterialIntersection nearest_intersection;

    for (auto mo: scene.objects) {
        auto intersection = mo->intersect(r);
        if (intersection && inInterval(scene.eps, nearest_intersection.t, intersection.t)) {
            nearest_intersection = intersection;
        }
    }

    return nearest_intersection;
}

glm::vec3 sceneIntersectColor(const Scene& scene, Ray r) {
    auto intersection = sceneIntersectMaterial(scene, r);

    if (!intersection) {
        return glm::vec3{0.0f};
    }

    return intersection.material->evaluate(intersection.input, scene);
}
