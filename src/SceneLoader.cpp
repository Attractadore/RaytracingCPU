#include "SceneLoader.hpp"
#include "Light.hpp"
#include "MaterialLoader.hpp"

#include <glm/gtc/matrix_transform.hpp>

NamedScene loadScene(const std::string&) {
    auto blue = getMaterial("BlueMaterial");
    auto marble = getMaterial("MarbleMaterial");
    auto grey = getMaterial("GreyMaterial");
    auto gold = getMaterial("GoldMaterial");

    auto sun = std::make_unique<DirectionalLight>(
        glm::vec3{10.0f, 10.0f, 9.0f},
        glm::normalize(glm::vec3{0.0f, -1.0f, -1.0f})
    );
    auto sphere = std::make_unique<Sphere>();
    auto plane = std::make_unique<Plane>();

    Model marble_sphere{sphere.get(), marble, {glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 1.0f})}};
    Model gold_sphere{sphere.get(), gold, glm::translate(glm::mat4(1.0f), {-3.0f, 2.0f, 1.0f})};
    Model blue_sphere{sphere.get(), blue, glm::translate(glm::mat4(1.0f), {-2.0f, -3.0f, 1.0f})};

    Model grey_plane{plane.get(), grey};

    Scene scene = {
        .lights = {},
        .meshes = {},
        .models = {marble_sphere, gold_sphere, blue_sphere, grey_plane},
        .camera = {
            .position{-10.0f, 0.0f, 5.0f},
            .direction = glm::normalize(glm::vec3{10.0f, 0.0f, -5.0f}),
            .up{0.0f, 0.0f, 1.0f},
        },
        .eps = 0.001f,
    };
    scene.lights.push_back(std::move(sun));
    scene.meshes.push_back(std::move(sphere));
    scene.meshes.push_back(std::move(plane));

    return {"Default", std::move(scene)};
}
