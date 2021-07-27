#include "Color.hpp"

#include <glm/exponential.hpp>

glm::vec3 srgbToLinear(glm::vec3 color, float gamma) {
    return glm::pow(color, glm::vec3{gamma});
}

glm::vec3 linearToSrgb(glm::vec3 color, float gamma) {
    return glm::pow(color, glm::vec3{1.0f / gamma});
}
