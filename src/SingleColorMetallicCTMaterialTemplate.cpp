#pragma once
#include "SingleColorCTMaterialTemplate.cpp"

template <
    glm::vec3 diffuse,
    float roughness>
struct SingleColorMetallicCTMaterial: public SingleColorCTMaterial<diffuse, roughness, 1.0f, 1.0f> {};
