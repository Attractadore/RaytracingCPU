#include "Lighting.hpp"

#include <glm/geometric.hpp>

glm::vec3 blinnPhong(
    glm::vec3 camera_dir,
    glm::vec3 light_dir,
    float light_intensity,
    glm::vec3 light_color,
    glm::vec3 diffuse,
    float specular_hardness,
    glm::vec3 normal) {
    glm::vec3 halfway_dir = glm::normalize(light_dir + camera_dir);
    float diffuse_strength = glm::max(glm::dot(normal, light_dir), 0.0f);
    float specular_strength = glm::pow(glm::max(glm::dot(normal, halfway_dir), 0.0f), specular_hardness);
    return light_intensity * (diffuse_strength * diffuse + specular_strength) * light_color;
}

