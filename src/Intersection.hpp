#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Model;

struct MeshIntersection {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    float t;

    explicit operator bool() const noexcept {
        return bool(t);
    }
};

struct ModelIntersection {
    const Model* model;
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    float t;

    ModelIntersection() noexcept;
    ModelIntersection(const Model* m, MeshIntersection is) noexcept;

    explicit operator bool() const {
        return model;
    }
};
