#include "Transform.hpp"

Transform::Transform(glm::mat4 m) {
    model = m;
    model_inv = glm::inverse(model);
    normal = glm::transpose(model_inv);
    normal_inv = glm::transpose(model);
}
