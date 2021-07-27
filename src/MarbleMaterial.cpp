#include "MaterialInterface.hpp"
#include "Light.hpp"
#include "Scene.hpp"
#include "Util/Lighting.hpp"
#include "Util/Math.hpp"
#include "Util/Color.hpp"

#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/noise.hpp>

struct Marble : public PerLightMaterial {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene, glm::vec3 eye_position, const Light* light) override;
};

glm::vec3 Marble::evaluate(MaterialInput input, const Scene& scene, glm::vec3 eye_position, const Light* light) {
    glm::vec3 position = input.position;
    glm::vec3 normal = input.normal;

    glm::vec3 light_dir = light->getDirection(position);
    float light_intensity = light->getIntensity(scene, position);

    glm::vec3 camera_dir = glm::normalize(eye_position - position);

    glm::vec3 diffuse_lf = srgbToLinear(glm::vec3{26.0f, 13.0f, 0.0f} / 255.0f);
    glm::vec3 diffuse_hf = srgbToLinear(glm::vec3{255.0f, 230.0f, 179.0f} / 255.0f);
    float mix_factor =
        0.62f * glm::perlin(2.0f * position) +
        0.25f * glm::perlin(4.0f * position) +
        0.13f * glm::perlin(8.0f * position) +
        glm::sin(4.0f * input.uv.s * glm::pi<float>());
    mix_factor = linearRemap(-1.0f, 1.0f, 0.0f, 1.0f, mix_factor);
    mix_factor = glm::clamp(mix_factor, 0.0f, 1.0f);
    glm::vec3 diffuse = glm::mix(diffuse_lf, diffuse_hf, mix_factor);
    float specular_hardness = 1024.0f;

    return blinnPhong(camera_dir, light_dir, light_intensity, light->color, diffuse, specular_hardness, normal);
}

std::unique_ptr<Material> makeMaterial() {
    return std::make_unique<Marble>();
}
