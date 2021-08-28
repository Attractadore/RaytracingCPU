#pragma once
#include "MaterialInterface.hpp"

template <
    glm::vec3 diffuse,
    glm::vec3 specular,
    float specular_hardness>
class SingleColorBPMaterial: public BlinnPhongMaterial {
protected:
    glm::vec3 getDiffuse(const MaterialInput&) const override { return diffuse; }
    glm::vec3 getSpecular(const MaterialInput&) const override { return specular; }
    float getSpecularHardness(const MaterialInput&) const override { return specular_hardness; }
};
