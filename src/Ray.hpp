#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

Ray rayTransform(Ray r, glm::mat4 t);

glm::vec3 rayAdvance(Ray r, float t);
