#pragma once
#include <glm/common.hpp>

float linearRemap(float left, float right, float new_left, float new_right, float x);

template<typename T>
bool inInterval(T left, T right, T x) {
    return left < x and x < right;
}

bool isNaNOrInf(float x);

template<typename VecType>
bool isNaNOrInf(VecType x) {
    return glm::any(glm::isnan(x)) || glm::any(glm::isinf(x));
}
