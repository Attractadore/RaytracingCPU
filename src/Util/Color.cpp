#include "Color.hpp"
#include "Math.hpp"

#include <glm/common.hpp>
#include <glm/exponential.hpp>

#include <numeric>

glm::vec3 srgbToLinear(glm::vec3 color, float gamma) {
    return glm::pow(color, glm::vec3{gamma});
}

glm::vec3 linearToSrgb(glm::vec3 color, float gamma) {
    return glm::pow(color, glm::vec3{1.0f / gamma});
}

glm::vec3 reinhardAverage(const std::vector<glm::vec3>& pixels) {
    glm::vec3 sum = std::accumulate(pixels.begin(), pixels.end(), glm::vec3{0.0f},
                                    [](glm::vec3 s, glm::vec3 p) {
                                        p = isNaNOrInf(p) ? glm::vec3{0.0f} : p;
                                        return s + glm::log2(0.01f + p);
                                    });
    return glm::max(glm::exp(sum / float(pixels.size())), 0.01f);
}

glm::vec3 reinhardToneMap(glm::vec3 pixel, glm::vec3 average, glm::vec3 middle_grey) {
    pixel = middle_grey * pixel / average;
    pixel = pixel / (1.0f + pixel);
    return pixel;
}
