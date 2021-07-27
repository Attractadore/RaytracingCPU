#include "Math.hpp"

#include <glm/common.hpp>

float linearRemap(float left, float right, float new_left, float new_right, float x) {
    float t = (x - left) / (right - left);
    return glm::mix(new_left, new_right, t);
}
