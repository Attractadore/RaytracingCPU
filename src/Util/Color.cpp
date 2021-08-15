#include "Color.hpp"

#include <glm/common.hpp>

glm::vec3 srgbToLinear(glm::vec3 color, float gamma) {
    return glm::pow(color, glm::vec3{gamma});
}

glm::vec3 linearToSrgb(glm::vec3 color, float gamma) {
    return glm::pow(color, glm::vec3{1.0f / gamma});
}

glm::vec3 reinhardToneMap(glm::vec3 pixel, glm::vec3 average, glm::vec3 middle_grey) {
    pixel = middle_grey * pixel / average;
    pixel = pixel / (1.0f + pixel);
    return pixel;
}
