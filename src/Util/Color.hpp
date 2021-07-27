#pragma once
#include <glm/vec3.hpp>

constexpr float default_gamma = 2.2f;

glm::vec3 srgbToLinear(glm::vec3 color, float gamma = default_gamma);
glm::vec3 linearToSrgb(glm::vec3 color, float gamma = default_gamma);
