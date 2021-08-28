#pragma once
#include "MaterialInterface.hpp"

template <
    glm::vec3 diffuse,
    float roughness,
    float metallic,
    float eta>
struct SingleColorCTMaterial: public virtual CookTorranceMaterial {
protected:
    glm::vec3 getDiffuse(const MaterialInput&) const override { return diffuse; }
    float getRoughness(const MaterialInput&) const override { return roughness; }
    float getMetallic(const MaterialInput&) const override { return metallic; }
    float getEta(const MaterialInput&) const override { return eta; }
};

template <
    glm::vec3 diffuse,
    float roughness>
struct SingleColorMetallicCTMaterial: public SingleColorCTMaterial<diffuse, roughness, 1.0f, 1.0f> {};

template <
    glm::vec3 diffuse,
    float roughness,
    float eta>
struct SingleColorNonMetallicCTMaterial: public SingleColorCTMaterial<diffuse, roughness, 0.0f, eta> {};
