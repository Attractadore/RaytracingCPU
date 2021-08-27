#pragma once

#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/vec3.hpp>

constexpr float default_gamma = 2.2f;

inline glm::vec3 srgbToLinear(glm::vec3 color, float gamma = default_gamma) {
    return glm::pow(color, glm::vec3{gamma});
}

inline glm::vec3 linearToSrgb(glm::vec3 color, float gamma = default_gamma) {
    return glm::pow(color, glm::vec3{1.0f / gamma});
}
