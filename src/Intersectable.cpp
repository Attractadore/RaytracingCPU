#include "Intersectable.hpp"
#include "Util/Math.hpp"

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

static MaterialInput sphereIntersection(Sphere s, glm::vec3 position) {
    float u = glm::atan(position.y, position.x) / glm::pi<float>();
    float v = glm::asin(position.z)  / glm::half_pi<float>();
    return {
        .position = s.trans.model * glm::vec4{position, 1.0f},
        .normal = glm::normalize(s.trans.normal * position),
        .uv{u, v},
    };
}

bool Sphere::hasIntersection(Ray r) {
    return bool{intersect(r)};
}

std::optional<float> Sphere::intersectDistance(Ray r) {
    auto intersection = intersect(r);
    if (intersection) {
        return intersection.value().t;
    }
    return {};
}

std::optional<Intersection> Sphere::intersect(Ray r) {
    r = rayTransform(r, trans.model_inv);

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
            return Intersection{sphereIntersection(*this, rayAdvance(r, n)), n};
        }
        float f = -b + sD;
        if (f > 0.0f) {
            return Intersection{sphereIntersection(*this, rayAdvance(r, f)), f};
        }
    }

    return {};
}

static MaterialInput planeIntersection(Plane p, glm::vec3 position) {
    return {
        .position = p.trans.model * glm::vec4{position, 1.0f},
        .normal = glm::normalize(p.trans.normal[2]),
    };
}

bool Plane::hasIntersection(Ray r) {
    return bool{intersect(r)};
}

std::optional<float> Plane::intersectDistance(Ray r) {
    auto intersection = intersect(r);
    if (intersection) {
        return intersection.value().t;
    }
    return {};
}

std::optional<Intersection> Plane::intersect(Ray r) {
    r = rayTransform(r, trans.model_inv);

    // (r.origin + t * r.direction) * (0, 0, 1) = 0
    float t = -r.origin.z / r.direction.z;

    if (t > 0.0f) {
        return Intersection{planeIntersection(*this, rayAdvance(r, t)), t};
    }
    return {};
}

std::optional<MaterialIntersection> MaterialIntersectable::intersect(Ray r) {
    auto is = object->intersect(r);
    if (is) {
        return MaterialIntersection{material, is.value()};
    }
    return {};
}
