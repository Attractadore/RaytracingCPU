#pragma once
#include "Camera.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

#include <optional>
#include <vector>

struct MaterialIntersectable;
struct Light;

struct Scene {
    std::vector<MaterialIntersectable*> objects;
    std::vector<Light*> lights;
    Camera camera;
    float eps;
};

bool sceneHasIntersection(const Scene& scene, Ray r);
std::optional<float> sceneIntersectDistance(const Scene& scene, Ray r);
std::optional<Intersection> sceneIntersect(const Scene& scene, Ray r);
std::optional<MaterialIntersection> sceneIntersectMaterial(const Scene& scene, Ray r);
glm::vec3 sceneIntersectColor(const Scene& scene, Ray r);
glm::vec3 sceneIntersectColor(const Scene& scene, Ray r, glm::vec3 eye_position);
