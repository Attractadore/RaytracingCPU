#pragma once
#include "Math.hpp"

#include <glm/exponential.hpp>
#include <glm/vec3.hpp>

#include <numeric>
#include <vector>

constexpr float default_gamma = 2.2f;
constexpr glm::vec3 default_middle_grey{0.5f};

glm::vec3 srgbToLinear(glm::vec3 color, float gamma = default_gamma);
glm::vec3 linearToSrgb(glm::vec3 color, float gamma = default_gamma);

template<typename ExecutionPolicy>
glm::vec3 reinhardAverage(ExecutionPolicy policy, const std::vector<glm::vec3>& pixels) {
    glm::vec3 sum = std::transform_reduce(
        policy, pixels.begin(), pixels.end(), glm::vec3{0.0f},
        std::plus(),
        [](glm::vec3 p) {
            p = isNaNOrInf(p) ? glm::vec3{0.0f} : p;
            return glm::log2(0.01f + p);
        }
    );
    return glm::max(glm::exp(sum / float(pixels.size())), 0.01f);
}

glm::vec3 reinhardToneMap(glm::vec3 pixel, glm::vec3 average, glm::vec3 middle_grey = default_middle_grey);
