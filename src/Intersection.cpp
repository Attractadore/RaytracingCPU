#include "Intersection.hpp"

#include <limits>

Intersection::Intersection():
    input{}, t{std::numeric_limits<float>::infinity()} {
}

MaterialIntersection::MaterialIntersection():
    material{}, input{}, t{std::numeric_limits<float>::infinity()} {
}
