#include "MaterialInterface.hpp"
#include "Util/Color.hpp"
#include "Util/Math.hpp"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/trigonometric.hpp>

class Marble: public MonteCarloCookTorranceMaterial {
protected:
    static constexpr float roughness = 0.5f;
    static constexpr float metallic = 0.0f;
    static constexpr float eta = 1.486f;

    glm::vec3 getDiffuse(const MaterialInput&) const noexcept override;
    float getRoughness(const MaterialInput&) const noexcept override { return roughness; }
    float getMetallic(const MaterialInput&) const noexcept override { return metallic; }
    float getEta(const MaterialInput&) const noexcept override { return eta; }
};

glm::vec3 Marble::getDiffuse(const MaterialInput& input) const noexcept {
    glm::vec3 diffuse_lf = srgbToLinear(glm::vec3{26.0f, 13.0f, 0.0f} / 255.0f);
    glm::vec3 diffuse_hf = srgbToLinear(glm::vec3{255.0f, 230.0f, 179.0f} / 255.0f);
    float mix_factor = 0.62f * glm::perlin(2.0f * input.position) +
                       0.25f * glm::perlin(4.0f * input.position) +
                       0.13f * glm::perlin(8.0f * input.position) +
                       glm::sin(4.0f * input.uv.s * glm::pi<float>());
    mix_factor = linearRemap(-1.0f, 1.0f, 0.0f, 1.0f, mix_factor);
    mix_factor = glm::clamp(mix_factor, 0.0f, 1.0f);
    return glm::mix(diffuse_lf, diffuse_hf, mix_factor);
}

const Material* getMaterial() {
    return getMaterialSubclass<Marble>();
}
