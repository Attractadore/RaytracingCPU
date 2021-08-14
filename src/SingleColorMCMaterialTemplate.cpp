#pragma once
#include "SingleColorCTMaterialTemplate.cpp"

template <
    glm::vec3 diffuse,
    float roughness,
    float metallic,
    float eta>
struct SingleColorMCMaterial: public SingleColorCTMaterial<diffuse, roughness, metallic, eta>, public MonteCarloCookTorranceMaterial {};

template <
    glm::vec3 diffuse,
    float roughness>
struct SingleColorMetallicMCMaterial: public SingleColorMCMaterial<diffuse, roughness, 1.0f, 1.0f> {};

template <
    glm::vec3 diffuse,
    float roughness,
    float eta>
struct SingleColorNonMetallicMCMaterial: public SingleColorMCMaterial<diffuse, roughness, 0.0f, eta> {};
