#pragma once
#include "Intersection.hpp"
#include "Ray.hpp"
#include "Transform.hpp"

class Material;

struct Mesh {
    virtual bool hasIntersection(Ray r, float eps) const {
        float t = intersectDistance(r);
        return bool(t) and t > eps;
    }
    virtual float intersectDistance(Ray r) const {
        return intersect(r).t;
    }
    virtual MeshIntersection intersect(Ray r) const = 0;
};

struct Sphere: public Mesh {
    virtual MeshIntersection intersect(Ray r) const override;
};

struct Plane: public Mesh {
    virtual MeshIntersection intersect(Ray r) const override;
};

struct Model {
    const Mesh* mesh;
    const Material* material;
    Transform t;

    bool hasIntersection(Ray r, float eps) const;
    float intersectDistance(Ray r) const;
    ModelIntersection intersect(Ray r) const;
};
