#include "Intersectable.hpp"
#include "Util/Math.hpp"

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

static MaterialInput sphereIntersection(Sphere s, glm::vec3 position) {
    float u = glm::atan(position.y, position.x) / glm::pi<float>();
    float v = glm::asin(position.z) / glm::half_pi<float>();
    return {
        .position = s.trans.model * glm::vec4{position, 1.0f},
        .normal = glm::normalize(s.trans.normal * position),
        .uv{u, v},
    };
}

bool Sphere::hasIntersection(Ray r) const {
    return bool{intersect(r)};
}

float Sphere::intersectDistance(Ray r) const {
    auto intersection = intersect(r);
    if (intersection) {
        return intersection.t;
    }
    return false;
}

Intersection Sphere::intersect(Ray r) const {
    r = rayTransform(r, trans.model_inv);

    // r.origin + t * r.direction = 1
    // r.origin ** 2 + 2 * t * r.origin * r.direction + t ** 2 * r.direction ** 2 = 1
    float a = glm::dot(r.direction, r.direction);
    float b = glm::dot(r.direction, r.origin) / a;
    float c = (glm::dot(r.origin, r.origin) - 1) / a;

    float D = b * b - c;

    if (D > 0.0f) {
        float sD = glm::sqrt(D);
        {
            float n = -b - sD;
            if (n > 0.0f) {
                Intersection intersection{
                    sphereIntersection(*this, rayAdvance(r, n)),
                    n};
                intersection.input.object = this;
                intersection.input.incident = rayTransform(r, trans.model).direction;
                intersection.input.bounces = r.bounces;
                return intersection;
            }
        }
        {
            float f = -b + sD;
            if (f > 0.0f) {
                Intersection intersection{
                    sphereIntersection(*this, rayAdvance(r, f)),
                    f};
                intersection.input.object = this;
                intersection.input.incident = rayTransform(r, trans.model).direction;
                intersection.input.bounces = r.bounces;
                return intersection;
            }
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

bool Plane::hasIntersection(Ray r) const {
    return bool{intersect(r)};
}

float Plane::intersectDistance(Ray r) const {
    auto intersection = intersect(r);
    if (intersection) {
        return intersection.t;
    }
    return false;
}

Intersection Plane::intersect(Ray r) const {
    r = rayTransform(r, trans.model_inv);

    // (r.origin + t * r.direction) * (0, 0, 1) = 0
    float t = -r.origin.z / r.direction.z;

    if (t > 0.0f) {
        Intersection intersection{
            planeIntersection(*this, rayAdvance(r, t)),
            t};
        intersection.input.object = this;
        intersection.input.incident = rayTransform(r, trans.model).direction;
        intersection.input.bounces = r.bounces;
        return intersection;
    }

    return {};
}

MaterialIntersection MaterialIntersectable::intersect(Ray r) const {
    return MaterialIntersection{material, object->intersect(r)};
}
