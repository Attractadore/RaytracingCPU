#pragma once
#include <glm/vec3.hpp>

glm::vec3 blinnPhong(
    glm::vec3 normal,
    glm::vec3 light_dir,
    glm::vec3 view_dir,
    glm::vec3 diffuse,
    glm::vec3 specular,
    float specular_hardness
);

glm::vec3 cookTorrance(
    glm::vec3 normal,
    glm::vec3 light_dir,
    glm::vec3 view_dir,
    glm::vec3 diffuse,
    float roughness,
    float metallic,
    float eta,
    float solid_angle
);

glm::vec3 cookTorranceAbstractLight(
    glm::vec3 normal,
    glm::vec3 light_dir,
    glm::vec3 view_dir,
    glm::vec3 diffuse,
    float roughness,
    float metallic,
    float eta
);
