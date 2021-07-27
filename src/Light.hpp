#pragma once

#include <glm/vec3.hpp>

struct Scene;

struct Light {
    glm::vec3 color;

    Light(glm::vec3 col):
        color{col} {}

    virtual glm::vec3 getDirection(glm::vec3 origin) const = 0;
    virtual float getIntensity(const Scene& scene, glm::vec3 origin) const = 0;
};

struct DirectionalLight: public Light {
    glm::vec3 direction;

    DirectionalLight(glm::vec3 col, glm::vec3 dir):
        Light{col}, direction{dir} {}

    virtual glm::vec3 getDirection(glm::vec3) const override;
    virtual float getIntensity(const Scene& scene, glm::vec3 position) const override;
};
