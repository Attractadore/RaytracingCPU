#include "RefractMaterialTemplate.cpp"

static constexpr float refractive_index = 1.5f;

struct RealGlass: public RefractiveMaterialTemplate<refractive_index> {
    virtual glm::vec3 evaluate(MaterialInput input, const Scene& scene) const override;
};

glm::vec3 RealGlass::evaluate(MaterialInput input, const Scene& scene) const {
    glm::vec3 reflect = glm::reflect(input.incident, input.normal);
    glm::vec3 reflect_color = sceneIntersectColor(scene, Ray{input.position, reflect});
    glm::vec3 refract_color = RefractiveMaterialTemplate<refractive_index>::evaluate(input, scene);
    return glm::mix(refract_color, reflect_color, 0.1f);
}

std::unique_ptr<Material> makeMaterial() {
    return std::make_unique<RealGlass>();
}
