#pragma once
#include "MaterialInput.hpp"

struct Intersectable;
class Material;

struct Intersection {
    MaterialInput input;
    float t;

    Intersection();
    Intersection(MaterialInput i, float d):
        input{i}, t{d} {}

    explicit operator bool() const {
        return input.object;
    }
};

struct MaterialIntersection {
    const Material* material;
    MaterialInput input;
    float t;

    MaterialIntersection();
    MaterialIntersection(const Material* m, Intersection is):
        material{m}, input{is.input}, t{is.t} {}

    explicit operator bool() const {
        return material && input.object;
    }
};
