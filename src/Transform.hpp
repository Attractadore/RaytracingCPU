#pragma once

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

struct Transform {
    glm::mat4 model;
    glm::mat4 model_inv;
    glm::mat3 normal;
    glm::mat3 normal_inv;

    Transform():
        model{1.0f}, model_inv{1.0f}, normal{1.0f}, normal_inv{1.0f} {};
    Transform(glm::mat4 m);
};
