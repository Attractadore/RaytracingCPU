#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Intersectable;

struct MaterialInput {
    const Intersectable* object;
    glm::vec3 incident;
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};
