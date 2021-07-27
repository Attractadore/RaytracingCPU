#pragma once
#include "MaterialInput.hpp"

#include <glm/vec3.hpp>

struct Scene;
struct Light;

struct Material {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene, glm::vec3 eye_position) = 0;
};

struct PerLightMaterial : public Material {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene, glm::vec3 eye_position) override;
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene, glm::vec3 eye_position, const Light* light) = 0;
};
