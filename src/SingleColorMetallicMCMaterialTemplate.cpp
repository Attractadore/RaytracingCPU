#pragma once
#include "SingleColorMCMaterialTemplate.cpp"

template <
    glm::vec3 diffuse,
    float roughness>
struct SingleColorMetallicMCMaterial: public SingleColorMCMaterial<diffuse, roughness, 1.0f, 1.0f> {};
