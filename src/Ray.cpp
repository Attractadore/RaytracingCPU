#include "Ray.hpp"

Ray::Ray(glm::vec3 o, glm::vec3 d): 
        origin{o}, direction{d}, bounces{1} {}

Ray::Ray(glm::vec3 o, glm::vec3 d, unsigned b):
        origin{o}, direction{d}, bounces{b} {}

Ray rayTransform(Ray r, glm::mat4 t) {
    return Ray{t * glm::vec4{r.origin, 1.0f},t * glm::vec4{r.direction, 0.0f}, r.bounces};
}

glm::vec3 rayAdvance(Ray r, float t) {
    return r.origin + t * r.direction;
}

Ray rayAdvanced(Ray r, float t) {
    return Ray{rayAdvance(r, t), r.direction, r.bounces};
}
