#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Model;

struct MaterialInput {
    const Model* model;
    glm::vec3 view;
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    unsigned bounces;
};
