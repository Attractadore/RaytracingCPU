#include "Light.hpp"
#include "Scene.hpp"

glm::vec3 DirectionalLight::getDirection(glm::vec3) const {
    return -direction;
}

float DirectionalLight::getIntensity(const Scene& scene, glm::vec3 origin) const {
    Ray r{origin, -direction};
    return !sceneHasIntersection(scene, r);
};
