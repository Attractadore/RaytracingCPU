#pragma once
#include "Light.hpp"
#include "Material.hpp"
#include "MaterialInterface.hpp"
#include "Scene.hpp"
#include "Util/Lighting.hpp"

template <
    glm::vec3 diffuse,
    float roughness,
    float metallic,
    float eta
>
struct SingleColorCTMaterial: public PerLightMaterial {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene, const Light* light) const override;
};

template <
    glm::vec3 diffuse,
    float roughness,
    float metallic,
    float eta
>
glm::vec3 SingleColorCTMaterial<diffuse, roughness, metallic, eta>::evaluate(MaterialInput input, const Scene& scene, const Light* light) const {
    glm::vec3 incident = input.incident;
    glm::vec3 position = input.position;
    glm::vec3 normal = input.normal;

    glm::vec3 light_dir = light->getDirection(position);
    float light_intensity = light->getIntensity(scene, position);

    return light_intensity * light->color * cookTorranceAbstractLight(normal, light_dir, -incident, diffuse, roughness, metallic, eta);
}
