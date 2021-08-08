#pragma once
#include "SingleColorMCMaterialTemplate.cpp"

template <
    glm::vec3 diffuse,
    float roughness,
    float eta>
struct SingleColorNonMetallicMCMaterial: public SingleColorMCMaterial<diffuse, roughness, 0.0f, eta> {};
