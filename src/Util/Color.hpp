#pragma once
#include <glm/vec3.hpp>

#include <vector>

constexpr float default_gamma = 2.2f;
constexpr glm::vec3 default_middle_grey{0.5f};

glm::vec3 srgbToLinear(glm::vec3 color, float gamma = default_gamma);
glm::vec3 linearToSrgb(glm::vec3 color, float gamma = default_gamma);

glm::vec3 reinhardAverage(const std::vector<glm::vec3>& pixels);
glm::vec3 reinhardToneMap(glm::vec3 pixel, glm::vec3 average, glm::vec3 middle_grey = default_middle_grey);
