#pragma once
#include "Intersection.hpp"
#include "Ray.hpp"
#include "Transform.hpp"

struct Intersectable {
    Transform trans;

    Intersectable(glm::mat4 model):
        trans{model} {}

    virtual bool hasIntersection(Ray r) const = 0;
    virtual float intersectDistance(Ray r) const = 0;
    virtual Intersection intersect(Ray r) const = 0;
};

struct Sphere: public Intersectable {
    Sphere():
        Sphere{glm::mat4{1.0f}} {}
    Sphere(glm::mat4 model):
        Intersectable{model} {}

    virtual bool hasIntersection(Ray r) const override;
    virtual float intersectDistance(Ray r) const override;
    virtual Intersection intersect(Ray r) const override;
};

struct Plane: Intersectable {
    Plane():
        Plane{glm::mat4{1.0f}} {}
    Plane(glm::mat4 model):
        Intersectable{model} {}

    virtual bool hasIntersection(Ray r) const override;
    virtual float intersectDistance(Ray r) const override;
    virtual Intersection intersect(Ray r) const override;
};

struct MaterialIntersectable {
    Intersectable* object;
    const Material* material;

    MaterialIntersection intersect(Ray r) const;
};
