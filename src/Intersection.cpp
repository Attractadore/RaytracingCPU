#include "Intersection.hpp"
#include "Intersectable.hpp"

#include <limits>

ModelIntersection::ModelIntersection() noexcept:
    model{nullptr},
    t{std::numeric_limits<float>::infinity()} {
}

ModelIntersection::ModelIntersection(const Model* m, MeshIntersection is) noexcept:
    model{is ? m : nullptr} {
    if (model) {
        position = model->t.model * glm::vec4{is.position, 1.0f};
        normal = model->t.normal * is.normal;
        uv = is.uv;
    }
    t = is.t;
}
