#include "Intersection.hpp"

#include <limits>

MaterialIntersection::MaterialIntersection():
    t{std::numeric_limits<float>::infinity()} {}
