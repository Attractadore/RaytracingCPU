#pragma once
#include <glm/vec3.hpp>

struct Scene;

glm::vec3 blinnPhong(
    glm::vec3 normal,
    glm::vec3 light_dir,
    glm::vec3 view_dir,
    glm::vec3 diffuse,
    glm::vec3 specular,
    float specular_hardness);

struct GGXSample {
    glm::vec3 halfway;
    float pdf;
};

GGXSample ggxImportanceSample(glm::vec3 normal, glm::vec3 view, float roughness);
GGXSample ggxVisibilityImportanceSample(glm::vec3 normal, glm::vec3 view, float roughness);

glm::vec3 cookTorrance(
    glm::vec3 normal,
    glm::vec3 light_dir,
    glm::vec3 view_dir,
    glm::vec3 diffuse,
    float roughness,
    float metallic,
    float eta);

glm::vec3 cookTorranceAbstractLight(
    glm::vec3 normal,
    glm::vec3 light_dir,
    glm::vec3 view_dir,
    glm::vec3 diffuse,
    float roughness,
    float metallic,
    float eta);
