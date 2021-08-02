#pragma once
#include "MaterialInput.hpp"

#include <glm/vec3.hpp>

struct Scene;
struct Light;

struct Material {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene) const = 0;
};

struct PerLightMaterial: public Material {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene) const override;
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene, const Light* light) const = 0;
};
