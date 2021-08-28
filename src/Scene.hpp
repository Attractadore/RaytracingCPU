#pragma once
#include "Camera.hpp"
#include "Intersectable.hpp"
#include "Ray.hpp"

#include <memory>
#include <vector>

struct Light;

struct Scene {
    std::vector<std::unique_ptr<Light>> lights;
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<Model> models;

    Camera camera;
    float eps;
};

bool sceneHasIntersection(const Scene& scene, Ray r);
float sceneIntersectDistance(const Scene& scene, Ray r);
ModelIntersection sceneIntersect(const Scene& scene, Ray r);
glm::vec3 sceneIntersectColor(const Scene& scene, Ray r);
