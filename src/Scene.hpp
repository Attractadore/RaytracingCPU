#pragma once
#include "Camera.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

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
float sceneIntersectDistance(const Scene& scene, Ray r);
Intersection sceneIntersect(const Scene& scene, Ray r);
MaterialIntersection sceneIntersectMaterial(const Scene& scene, Ray r);
glm::vec3 sceneIntersectColor(const Scene& scene, Ray r);
