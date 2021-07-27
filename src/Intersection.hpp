#pragma once
#include "MaterialInput.hpp"

struct Intersectable;
struct Material;

struct Intersection {
    MaterialInput input;
    float t;
};

struct MaterialIntersection {
    Material* material;
    MaterialInput input;
    float t;

    MaterialIntersection();
    MaterialIntersection(Material* m, Intersection is):
        material{m}, input{is.input}, t{is.t} {}
};
