#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

struct Camera {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
};

glm::mat4 cameraView(Camera camera);
