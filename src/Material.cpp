#include "Material.hpp"
#include "Util/Lighting.hpp"
#include "Scene.hpp"
#include "Light.hpp"

glm::vec3 BlinnPhongMaterial::evaluate(MaterialInput input, const Scene& scene) const {
    glm::vec3 position = getPosition(input);
    glm::vec3 normal = getNormal(input);
    glm::vec3 view = getView(input);
    glm::vec3 diffuse = getDiffuse(input);
    glm::vec3 specular = getSpecular(input);
    float specular_hardness = getSpecularHardness(input);
    return evaluate(input, scene, position, normal, view, diffuse, specular, specular_hardness);
}

glm::vec3 BlinnPhongMaterial::evaluate(MaterialInput, const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, glm::vec3 specular, float specular_hardness) const noexcept {
    glm::vec3 color{0.0f};
    for (auto light: scene.lights) {
        color += evaluate(scene, position, normal, view, diffuse, specular, specular_hardness, light);
    }
    return color;
}

glm::vec3 BlinnPhongMaterial::evaluate(const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, glm::vec3 specular, float specular_hardness, const Light* light) const noexcept {
    float light_intencity = light->getIntensity(scene, position);
    glm::vec3 light_dir = light->getDirection(position);
    return light_intencity *
           light->color *
           blinnPhong(normal, light_dir, view, diffuse, specular, specular_hardness);
}

glm::vec3 CookTorranceMaterial::evaluate(MaterialInput input, const Scene& scene) const {
    glm::vec3 position = getPosition(input);
    glm::vec3 normal = getNormal(input);
    glm::vec3 view = getView(input);
    glm::vec3 diffuse = getDiffuse(input);
    float roughness = getRoughness(input);
    float metallic = getMetallic(input);
    float eta = getEta(input);
    return evaluate(input, scene, position, normal, view, diffuse, roughness, metallic, eta);
}

glm::vec3 CookTorranceMaterial::evaluate(MaterialInput, const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, float roughness, float metallic, float eta) const noexcept {
    glm::vec3 color{0.0f};
    for (auto light: scene.lights) {
        color += evaluate(scene, position, normal, view, diffuse, roughness, metallic, eta, light);
    }
    return color;
}

glm::vec3 CookTorranceMaterial::evaluate(const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, float roughness, float metallic, float eta, const Light* light) const noexcept {
    float light_intencity = light->getIntensity(scene, position);
    glm::vec3 light_dir = light->getDirection(position);
    return light_intencity *
           light->color *
           cookTorranceAbstractLight(normal, light_dir, view, diffuse, roughness, metallic, eta);
}

glm::vec3 MonteCarloCookTorranceMaterial::evaluate(MaterialInput input, const Scene& scene, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, float roughness, float metallic, float eta) const noexcept {
    return cookTorranceGlobalIllumination(scene, input.bounces, position, normal, view, diffuse, roughness, metallic, eta) +
           CookTorranceMaterial::evaluate(input, scene, position, normal, view, diffuse, roughness, metallic, eta);
}
