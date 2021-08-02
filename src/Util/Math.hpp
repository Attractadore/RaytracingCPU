#pragma once

float linearRemap(float left, float right, float new_left, float new_right, float x);

template<typename T>
bool inInterval(T left, T right, T x) {
    return left < x and x < right;
}
