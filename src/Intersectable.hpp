#pragma once
#include "Ray.hpp"
#include "Transform.hpp"
#include "Intersection.hpp"

#include <optional>

struct Intersectable {
    Transform trans;

    Intersectable(glm::mat4 model):
        trans{model} {}

    virtual bool hasIntersection(Ray r) = 0;
    virtual std::optional<float> intersectDistance(Ray r) = 0;
    virtual std::optional<Intersection> intersect(Ray r) = 0;
};

struct Sphere: public Intersectable {
    Sphere():
        Sphere{glm::mat4{1.0f}} {}
    Sphere(glm::mat4 model):
        Intersectable{model} {}

    virtual bool hasIntersection(Ray r) override;
    virtual std::optional<float> intersectDistance(Ray r) override;
    virtual std::optional<Intersection> intersect(Ray r) override;
};

struct Plane: Intersectable {
    Plane():
        Plane{glm::mat4{1.0f}} {}
    Plane(glm::mat4 model):
        Intersectable{model} {}

    virtual bool hasIntersection(Ray r) override;
    virtual std::optional<float> intersectDistance(Ray r) override;
    virtual std::optional<Intersection> intersect(Ray r) override;
};

struct MaterialIntersectable {
    Intersectable* object;
    Material* material;

    std::optional<MaterialIntersection> intersect(Ray r);
};
