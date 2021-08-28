#include "Intersectable.hpp"

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

namespace {
MeshIntersection sphereIntersection(glm::vec3 position, float t) {
    float u = glm::atan(position.y, position.x) / glm::pi<float>();
    float v = glm::asin(position.z) / glm::half_pi<float>();
    return {
        .position{position},
        .normal{position},
        .uv{u, v},
        .t{t},
    };
}

MeshIntersection planeIntersection(glm::vec3 position, float t) {
    return {
        .position{position},
        .normal{0.0f, 0.0f, 1.0f},
        .uv{position.x, position.y},
        .t{t},
    };
}
}  // namespace

MeshIntersection Sphere::intersect(Ray r) const {
    // r.origin + t * r.direction = 1
    // r.origin ** 2 + 2 * t * r.origin * r.direction + t ** 2 * r.direction ** 2 = 1
    float a = glm::dot(r.direction, r.direction);
    float b = glm::dot(r.direction, r.origin) / a;
    float c = (glm::dot(r.origin, r.origin) - 1) / a;

    float D = b * b - c;

    if (D > 0.0f) {
        float sD = glm::sqrt(D);
        float n = -b - sD;
        if (n > 0.0f) {
            return sphereIntersection(rayAdvance(r, n), n);
        }
        float f = -b + sD;
        if (f > 0.0f) {
            return sphereIntersection(rayAdvance(r, f), f);
        }
    }
    return {.t{false}};
}

MeshIntersection Plane::intersect(Ray r) const {
    // (r.origin + t * r.direction) * (0, 0, 1) = 0
    float t = -r.origin.z / r.direction.z;

    if (t > 0.0f) {
        return planeIntersection(rayAdvance(r, t), t);
    }

    return {.t{false}};
}

bool Model::hasIntersection(Ray r, float eps) const {
    r = rayTransform(r, t.model_inv);
    return mesh->hasIntersection(r, eps);
}

float Model::intersectDistance(Ray r) const {
    r = rayTransform(r, t.model_inv);
    return mesh->intersectDistance(r);
}

ModelIntersection Model::intersect(Ray r) const {
    r = rayTransform(r, t.model_inv);
    return ModelIntersection{this, mesh->intersect(r)};
}
