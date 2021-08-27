#pragma once
#include <glm/common.hpp>

inline float linearRemap(float left, float right, float new_left, float new_right, float x) {
    float t = (x - left) / (right - left);
    return glm::mix(new_left, new_right, t);
}

template <typename T>
bool inInterval(T left, T right, T x) {
    return left < x and x < right;
}

inline bool isNaNOrInf(float x) {
    return glm::isnan(x) || glm::isinf(x);
}

template <typename VecType>
bool isNaNOrInf(VecType x) {
    return glm::any(glm::isnan(x)) || glm::any(glm::isinf(x));
}
