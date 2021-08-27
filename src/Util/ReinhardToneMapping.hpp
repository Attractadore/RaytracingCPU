#pragma once
#include "ExecutionPolicy.hpp"
#include "Math.hpp"
#include "PixelIterator.hpp"

#include <numeric>
#include <ranges>

constexpr glm::vec3 default_middle_grey{0.5f};

template <ExecutionPolicy P, PixelForwardIterator I>
glm::vec3 reinhardAverage(P policy, I first, I last) {
    glm::vec3 sum = std::transform_reduce(
        policy, first, last, glm::vec3{0.0f},
        std::plus(),
        [](glm::vec3 pixel) {
            pixel = isNaNOrInf(pixel) ? glm::vec3{0.0f} : pixel;
            return glm::log2(0.01f + pixel);
        });
    auto size = std::distance(first, last);
    return glm::max(glm::exp(sum / float(size)), 0.01f);
}

inline glm::vec3 reinhardToneMap(glm::vec3 pixel, glm::vec3 average, glm::vec3 middle_grey = default_middle_grey) {
    pixel = middle_grey * pixel / average;
    pixel = pixel / (1.0f + pixel);
    return pixel;
}

template <ExecutionPolicy P, PixelForwardIterator I>
auto reinhardToneMapPixels(P policy, I first, I last) {
    glm::vec3 average = reinhardAverage(policy, first, last);
    return std::views::transform(
        std::ranges::subrange(first, last),
        [=](glm::vec3 pixel) { return reinhardToneMap(pixel, average); });
}
