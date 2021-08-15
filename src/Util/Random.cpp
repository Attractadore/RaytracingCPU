#include "Random.hpp"

#include <random>

float canonicalRandom() {
    static std::mt19937 gen{std::random_device{}()};
    static std::uniform_real_distribution dis{0.0f, 1.0f};
    return dis(gen);
}
