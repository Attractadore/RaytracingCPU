#pragma once
#include "MaterialInput.hpp"

#include <glm/vec3.hpp>

struct Scene;
struct Light;

class Material {
public:
    virtual glm::vec3 evaluate(const MaterialInput& input, const Scene& scene) const = 0;

protected:
    virtual glm::vec3 getPosition(const MaterialInput& input) const { return input.position; }
    virtual glm::vec3 getNormal(const MaterialInput& input) const { return input.normal; }
    virtual glm::vec3 getView(const MaterialInput& input) const { return input.view; }
};

class BlinnPhongMaterial: public Material {
public:
    virtual glm::vec3 evaluate(const MaterialInput& input, const Scene& scene) const override;

protected:
    virtual glm::vec3 getDiffuse(const MaterialInput& input) const = 0;
    virtual glm::vec3 getSpecular(const MaterialInput& input) const = 0;
    virtual float getSpecularHardness(const MaterialInput& input) const = 0;

    virtual glm::vec3 evaluate(const MaterialInput& input, const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, glm::vec3 specular, float specular_hardness) const;
    virtual glm::vec3 evaluate(const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, glm::vec3 specular, float specular_hardness, const Light* light) const;
};

class CookTorranceMaterial: public Material {
public:
    virtual glm::vec3 evaluate(const MaterialInput& input, const Scene& scene) const override;

protected:
    virtual glm::vec3 getDiffuse(const MaterialInput& input) const = 0;
    virtual float getRoughness(const MaterialInput& input) const = 0;
    virtual float getMetallic(const MaterialInput& input) const = 0;
    virtual float getEta(const MaterialInput& input) const = 0;

    virtual glm::vec3 evaluate(const MaterialInput& input, const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, float roughness, float metallic, float eta) const;
    virtual glm::vec3 evaluate(const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, float roughness, float metallic, float eta, const Light* light) const;
};

class MonteCarloCookTorranceMaterial: public virtual CookTorranceMaterial {
protected:
    virtual glm::vec3 evaluate(const MaterialInput& input, const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, float roughness, float metallic, float eta) const override;
};
