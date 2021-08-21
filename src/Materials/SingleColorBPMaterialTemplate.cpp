#pragma once
#include "MaterialInterface.hpp"

template <
    glm::vec3 diffuse,
    glm::vec3 specular,
    float specular_hardness>
class SingleColorBPMaterial: public BlinnPhongMaterial {
protected:
    glm::vec3 getDiffuse(MaterialInput) const noexcept override { return diffuse; }
    glm::vec3 getSpecular(MaterialInput) const noexcept override { return specular; }
    float getSpecularHardness(MaterialInput) const noexcept override { return specular_hardness; }
};
