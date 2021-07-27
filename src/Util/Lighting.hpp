#pragma once
#include <glm/vec3.hpp>

glm::vec3 blinnPhong(
    glm::vec3 camera_dir,
    glm::vec3 light_dir,
    float light_intensity,
    glm::vec3 light_color,
    glm::vec3 diffuse,
    float specular_hardness,
    glm::vec3 normal);
