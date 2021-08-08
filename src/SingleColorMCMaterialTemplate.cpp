#pragma once
#include "SingleColorCTMaterialTemplate.cpp"
#include "Util/Lighting.hpp"

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

template <
    glm::vec3 diffuse,
    float roughness,
    float metallic,
    float eta
>
struct SingleColorMCMaterial: public SingleColorCTMaterial<diffuse, roughness, metallic, eta> {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene) const override;
};

template <
    glm::vec3 diffuse,
    float roughness,
    float metallic,
    float eta
>
glm::vec3 SingleColorMCMaterial<diffuse, roughness, metallic, eta>::evaluate(MaterialInput input, const Scene& scene) const {
    glm::vec3 color{0.0f};
    if (input.bounces > 1) {
        glm::vec3 incident = input.incident;
        glm::vec3 position = input.position;
        glm::vec3 normal = input.normal;
        GGXSample sample = ggxImportanceSample(normal, -incident, roughness);
        glm::vec3 reflect = glm::reflect(-incident, sample.normal);
        Ray r{position, reflect, input.bounces - 1};
        color = sceneIntersectColor(scene, r) * cookTorrance(normal, reflect, -incident, diffuse, roughness, metallic, eta, glm::two_pi<float>()) / sample.pdf;
    }
    color += PerLightMaterial::evaluate(input, scene);
    return color;
}
