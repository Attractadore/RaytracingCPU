#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 cameraView(Camera camera) {
    return glm::lookAt(camera.position, camera.position + camera.direction, camera.up);
}
