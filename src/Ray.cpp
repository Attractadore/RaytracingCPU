#include "Ray.hpp"

Ray rayTransform(Ray r, glm::mat4 t) {
    return {
        .origin = t * glm::vec4{r.origin, 1.0f},
        .direction = t * glm::vec4{r.direction, 0.0f},
    };
}

glm::vec3 rayAdvance(Ray r, float t) {
    return r.origin + t * r.direction;
}
