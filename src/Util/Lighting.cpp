#include "Lighting.hpp"
#include "../Scene.hpp"
#include "Math.hpp"

#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include <random>

glm::vec3 blinnPhong(glm::vec3 normal, glm::vec3 light, glm::vec3 view, glm::vec3 diffuse, glm::vec3 specular, float specular_hardness) {
    glm::vec3 halfway = glm::normalize(light + view);
    float n_dot_l = glm::max(glm::dot(normal, light), 0.0f);
    float n_dot_h = glm::max(glm::dot(normal, halfway), 0.0f);
    float kd = n_dot_l;
    float ks = glm::pow(n_dot_h, specular_hardness);
    return kd * diffuse + ks * specular;
}

namespace {
std::random_device rd;
std::mt19937 gen{rd()};
std::uniform_real_distribution dis{0.0f, 1.0f};
constexpr float eps = 0.001f;

float ggxLambda(glm::vec3 n, glm::vec3 s, float alpha) {
    float n_dot_s = glm::dot(n, s);
    float n_dot_s2 = n_dot_s * n_dot_s;
    float alpha2 = alpha * alpha;
    float a2 = glm::max(n_dot_s2 / (alpha2 * (1.0f - n_dot_s2)), eps);
    return (glm::sqrt(1.0f + 1.0f / a2) - 1.0f) / 2.0f;
}

float ggxGeometry(glm::vec3 n, glm::vec3 l, glm::vec3 v, float a) {
    return 1.0f / (1.0f + ggxLambda(n, l, a) + ggxLambda(n, v, a));
}

float ggxDistribution(glm::vec3 n, glm::vec3 m, float a) {
    assert(!isNaNOrInf(n));
    assert(!isNaNOrInf(m));
    assert(!isNaNOrInf(a));

    float n_dot_m = glm::dot(n, m);
    float n_dot_m2 = n_dot_m * n_dot_m;
    float a2 = a * a;

    float nom = a2;

    float denom = 1.0f + n_dot_m2 * (a2 - 1.0f);
    denom = glm::max(glm::pi<float>() * denom * denom, eps);
    assert(!isNaNOrInf(denom));

    return nom / denom;
}

float ggxPDF(glm::vec3 n, glm::vec3 m, float a) {
    float n_dot_m = glm::dot(n, m);
    return ggxDistribution(n, m, a) * n_dot_m;
}

glm::vec3 fresnel(glm::vec3 n, glm::vec3 v, glm::vec3 f0) {
    float n_dot_v = glm::dot(n, v);
    return f0 + (1.0f - f0) * glm::pow(1 - n_dot_v, 5.0f);
}

float f0Dielectric(float eta) {
    float f0 = (eta - 1.0f) / (eta + 1.0f);
    return f0 * f0;
}

glm::vec3 f0Metal(glm::vec3 diffuse) {
    return diffuse;
}

glm::vec3 fresnel(glm::vec3 n, glm::vec3 v, float eta, glm::vec3 diffuse, float metallic) {
    return fresnel(n, v, glm::mix(glm::vec3{f0Dielectric(eta)}, f0Metal(diffuse), metallic));
}

float microfacetBRDF(glm::vec3 n, glm::vec3 l, glm::vec3 v, float roughness) {
    assert(!isNaNOrInf(n));
    assert(!isNaNOrInf(l));
    assert(!isNaNOrInf(v));
    assert(!isNaNOrInf(roughness));

    float a = roughness * roughness;
    glm::vec3 m = glm::normalize(l + v);
    assert(!isNaNOrInf(m));
    float n_dot_l = glm::dot(n, l);
    float n_dot_v = glm::dot(n, v);

    float D = ggxDistribution(n, m, a);
    float G = ggxGeometry(n, l, v, a);
    float nom = G * D;
    assert(!isNaNOrInf(nom));

    float denom = glm::max(4.0f * n_dot_l * n_dot_v, eps);
    assert(!isNaNOrInf(denom));

    return glm::min(nom / denom, 1.0f);
}

glm::vec3 lambertDiffuse(glm::vec3 diffuse) {
    return diffuse / glm::pi<float>();
}
}  // namespace

GGXSample ggxImportanceSample(glm::vec3 normal, glm::vec3 view, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;

    float rand_theta = dis(gen);
    float rand_phi = dis(gen);

    float cos_theta2 = (1.0f - rand_theta) / (rand_theta * (a2 - 1.0f) + 1.0f);
    float sin_theta2 = 1.0f - cos_theta2;
    float cos_theta = glm::sqrt(cos_theta2);
    float sin_theta = glm::sqrt(sin_theta2);

    float phi = glm::two_pi<float>() * rand_phi;
    float cos_phi = glm::cos(phi);
    float sin_phi = glm::sin(phi);

    glm::vec3 halfway{sin_theta * cos_phi, sin_theta * sin_phi, cos_theta};
    glm::vec3 default_normal{0.0f, 0.0f, 1.0f};
    glm::vec3 axis = glm::cross(default_normal, normal);
    float axis_length = glm::length(axis);
    if (axis_length < eps) {
        axis = glm::vec3{0.0f, 1.0f, 0.0f};
    } else {
        axis = axis / axis_length;
    }
    float angle = glm::acos(glm::dot(default_normal, normal));
    glm::mat3 rotate = glm::rotate(glm::mat4{1.0f}, angle, axis);
    halfway = rotate * halfway;
    assert(!isNaNOrInf(halfway));

    float pdf = ggxPDF(normal, halfway, a);
    float h_dot_v = glm::max(glm::dot(halfway, view), eps);
    pdf = glm::max(pdf / (4.0f * h_dot_v), eps);
    assert(!isNaNOrInf(pdf));

    return {.halfway = halfway, .pdf = pdf};
}

glm::vec3 cookTorrance(glm::vec3 normal, glm::vec3 light, glm::vec3 view, glm::vec3 diffuse, float roughness, float metallic, float eta) {
    glm::vec3 ks = fresnel(normal, view, eta, diffuse, metallic);
    glm::vec3 kd = (1.0f - ks) * (1.0f - metallic);
    float n_dot_l = glm::dot(normal, light);
    float n_dot_v = glm::dot(normal, view);
    if (n_dot_l > 0.0f && n_dot_v > 0.0f) {
        glm::vec3 diffuse_term = lambertDiffuse(diffuse);
        float specular_term = microfacetBRDF(normal, light, view, roughness);
        return (kd * diffuse_term + ks * specular_term) * n_dot_l;
    } else {
        return glm::vec3{0.0f};
    }
}

glm::vec3 cookTorranceAbstractLight(glm::vec3 normal, glm::vec3 light, glm::vec3 view, glm::vec3 diffuse, float roughness, float metallic, float eta) {
    return cookTorrance(normal, light, view, diffuse, roughness, metallic, eta) * glm::pi<float>();
}

glm::vec3 cookTorranceGlobalIllumination(const Scene& scene, unsigned bounces, glm::vec3 position, glm::vec3 normal, glm::vec3 view, glm::vec3 diffuse, float roughness, float metallic, float eta) {
    if (bounces > 1) {
        auto [halfway, pdf] = ggxImportanceSample(normal, view, roughness);
        glm::vec3 reflect = glm::reflect(-view, halfway);
        Ray r{position, reflect, bounces - 1};
        return sceneIntersectColor(scene, r) *
               cookTorrance(normal, reflect, view, diffuse, roughness, metallic, eta) / pdf;
    } else {
        return glm::vec3{0.0f};
    }
}
