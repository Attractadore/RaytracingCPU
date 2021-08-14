#pragma once
#include "MaterialInput.hpp"

#include <glm/vec3.hpp>

struct Scene;
struct Light;

class Material {
public:
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene) const = 0;

protected:
    virtual glm::vec3 getPosition(MaterialInput input) const noexcept { return input.position; }
    virtual glm::vec3 getNormal(MaterialInput input) const noexcept { return input.normal; }
    virtual glm::vec3 getView(MaterialInput input) const noexcept { return -input.incident; }
};

class BlinnPhongMaterial: public Material {
public:
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene) const override;

protected:
    virtual glm::vec3 getDiffuse(MaterialInput input) const noexcept = 0;
    virtual glm::vec3 getSpecular(MaterialInput input) const noexcept = 0;
    virtual float getSpecularHardness(MaterialInput input) const noexcept = 0;

    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, glm::vec3 specular, float specular_hardness) const noexcept;
    virtual glm::vec3 evaluate(const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, glm::vec3 specular, float specular_hardness, const Light* light) const noexcept;
};

class CookTorranceMaterial: public Material {
public:
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene) const override;

protected:
    virtual glm::vec3 getDiffuse(MaterialInput input) const noexcept = 0;
    virtual float getRoughness(MaterialInput input) const noexcept = 0;
    virtual float getMetallic(MaterialInput input) const noexcept = 0;
    virtual float getEta(MaterialInput input) const noexcept = 0;

    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, float roughness, float metallic, float eta) const noexcept;
    virtual glm::vec3 evaluate(const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, float roughness, float metallic, float eta, const Light* light) const noexcept;
};

class MonteCarloCookTorranceMaterial : public virtual CookTorranceMaterial {
protected:
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, float roughness, float metallic, float eta) const noexcept override;
};
