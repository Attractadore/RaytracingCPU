#pragma once
#include "SingleColorCTMaterialTemplate.cpp"

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

#include <random>

template <
    glm::vec3 diffuse,
    float roughness,
    float metallic,
    float eta
>
struct SingleColorMCMaterial: public SingleColorCTMaterial<diffuse, roughness, metallic, eta> {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene) const override;
};

glm::vec3 randomDirection(glm::vec3 normal) {
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution dis{-1.0f, 1.0f};
    float theta = glm::pi<float>() * dis(gen);
    float z = dis(gen);
    float u = glm::sqrt(1.0f - z * z);
    glm::vec3 dir{u * glm::cos(theta), u * glm::sin(theta), z};
    if (glm::dot(normal, dir) < 0.0f) {
        dir = -dir;
    }
    return dir;
}

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
        Ray r{position, randomDirection(normal), input.bounces - 1};
        color += sceneIntersectColor(scene, r) * cookTorrance(normal, r.direction, -incident, diffuse, roughness, metallic, eta, glm::two_pi<float>());
    }
    //color += SingleColorCTMaterial<diffuse, roughness, metallic, eta>::evaluate(input, scene);
    color += PerLightMaterial::evaluate(input, scene);
    return color;
}
