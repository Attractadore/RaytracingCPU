#pragma once
#include "SingleColorCTMaterialTemplate.cpp"

template <
    glm::vec3 diffuse,
    float roughness,
    float eta>
struct SingleColorNonMetallicCTMaterial: public SingleColorCTMaterial<diffuse, roughness, 0.0f, eta> {};
