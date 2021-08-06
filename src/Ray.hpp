#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
    unsigned bounces;

    Ray(glm::vec3 o, glm::vec3 d);
    Ray(glm::vec3 o, glm::vec3 d, unsigned b);
};

Ray rayTransform(Ray r, glm::mat4 t);

glm::vec3 rayAdvance(Ray r, float t);
Ray rayAdvanced(Ray r, float t);
