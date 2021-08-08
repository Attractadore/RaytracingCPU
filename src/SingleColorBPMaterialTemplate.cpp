#pragma once
#include "Light.hpp"
#include "Material.hpp"
#include "MaterialInterface.hpp"
#include "Scene.hpp"
#include "Util/Lighting.hpp"

template <
    glm::vec3 diffuse,
    glm::vec3 specular,
    float specular_hardness>
struct SingleColorBPMaterial: public PerLightMaterial {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene, const Light* light) const override;
};

template <
    glm::vec3 diffuse,
    glm::vec3 specular,
    float specular_hardness>
glm::vec3 SingleColorBPMaterial<diffuse, specular, specular_hardness>::evaluate(MaterialInput input, const Scene& scene, const Light* light) const {
    glm::vec3 incident = input.incident;
    glm::vec3 position = input.position;
    glm::vec3 normal = input.normal;

    glm::vec3 light_dir = light->getDirection(position);
    float light_intensity = light->getIntensity(scene, position);

    return light_intensity * light->color * blinnPhong(normal, light_dir, -incident, diffuse, specular, specular_hardness);
}
