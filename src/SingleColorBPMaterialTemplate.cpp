#pragma once
#include "Material.hpp"
#include "Light.hpp"
#include "Scene.hpp"
#include "Util/Lighting.hpp"
#include "MaterialInterface.hpp"

template<
    glm::vec3 diffuse,
    float specular_hardness
>
struct SingleColorBPMaterial: public PerLightMaterial {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene, glm::vec3 eye_position, const Light* light) override;
};

template<
    glm::vec3 diffuse,
    float specular_hardness
>
glm::vec3 SingleColorBPMaterial<diffuse, specular_hardness>::evaluate(MaterialInput input, const Scene& scene, glm::vec3 eye_position, const Light* light) {
    glm::vec3 position = input.position;
    glm::vec3 normal = input.normal;

    glm::vec3 light_dir = light->getDirection(position);
    float light_intensity = light->getIntensity(scene, position);

    glm::vec3 camera_dir = glm::normalize(eye_position - position);

    return blinnPhong(camera_dir, light_dir, light_intensity, light->color, diffuse, specular_hardness, normal);
}
