#include "Lighting.hpp"
#include "Random.hpp"

#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

glm::vec3 blinnPhong(glm::vec3 normal, glm::vec3 light, glm::vec3 view, glm::vec3 diffuse, glm::vec3 specular, float specular_hardness) {
    glm::vec3 halfway = glm::normalize(light + view);
    float n_dot_l = glm::max(glm::dot(normal, light), 0.0f);
    float n_dot_h = glm::max(glm::dot(normal, halfway), 0.0f);
    float kd = n_dot_l;
    float ks = glm::pow(n_dot_h, specular_hardness);
    return kd * diffuse + ks * specular;
}

namespace {
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

float ggxMasking(glm::vec3 n, glm::vec3 s, float alpha) {
    return 1.0f / (1.0f + ggxLambda(n, s, alpha));
}

float ggxDistribution(glm::vec3 n, glm::vec3 m, float alpha) {
    float n_dot_m = glm::dot(n, m);
    float n_dot_m2 = n_dot_m * n_dot_m;
    float alpha2 = alpha * alpha;
    float nom = alpha2;
    float denom = 1.0f + n_dot_m2 * (alpha2 - 1.0f);
    denom = glm::max(glm::pi<float>() * denom * denom, eps);
    return nom / denom;
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

float reflectJacobian(glm::vec3 n, glm::vec3 s) {
    float n_dot_s = glm::dot(n, s);
    return 4.0f * n_dot_s;
}

glm::vec3 ggxGenerateNormal(glm::vec3 n, float alpha, float rand_theta, float rand_phi) {
    float alpha2 = alpha * alpha;

    glm::vec3 up{0.0f, 0.0f, 1.0f};
    glm::vec3 t = glm::cross(up, n);
    float t_length = glm::length(t);
    t = (t_length < eps) ? glm::vec3{0.0f, 1.0f, 0.0f} : t / t_length;
    glm::vec3 b = glm::normalize(glm::cross(n, t));

    float cos_theta2 = (1.0f - rand_theta) / (rand_theta * (alpha2 - 1.0f) + 1.0f);
    float sin_theta2 = 1.0f - cos_theta2;
    float cos_theta = glm::sqrt(cos_theta2);
    float sin_theta = glm::sqrt(sin_theta2);

    float phi = glm::two_pi<float>() * rand_phi;

    glm::vec3 m = sin_theta * glm::cos(phi) * t +
                  sin_theta * glm::sin(phi) * b +
                  cos_theta * n;

    return m;
}

float ggxPDF(glm::vec3 n, glm::vec3 m, glm::vec3 v, float alpha) {
    float n_dot_m = glm::dot(n, m);
    float nom = ggxDistribution(n, m, alpha) * n_dot_m;
    float denom = glm::max(reflectJacobian(m, v), eps);
    float pdf = glm::max(nom / denom, eps);
    return pdf;
}

glm::vec3 ggxVisibilityGenerateNormal(glm::vec3 n, glm::vec3 v, float alpha, float rand_r, float rand_phi) {
    v = glm::normalize(glm::mix(glm::dot(n, v) * n, v, alpha));
    glm::vec3 t = glm::normalize(glm::cross(v, n));
    glm::vec3 b = glm::cross(t, v);

    float z = glm::dot(n, v);
    float a = 1.0f / (1.0f + z);

    float r = glm::sqrt(rand_r);
    float phi = ((rand_phi < a) ? rand_phi / a : (rand_phi - a) / (1.0f - a) + 1.0f) *
                glm::pi<float>();

    glm::vec3 p = r * (glm::cos(phi) * t +
                       glm::sin(phi) * ((rand_phi < a) ? 1.0f : z) * b);
    glm::vec3 m = p + glm::sqrt(glm::max(1.0f - glm::dot(p, p), 0.0f)) * v;
    m = glm::normalize(glm::mix(glm::dot(n, m) * n, m, alpha));

    return m;
}

float ggxVisibilityPDF(glm::vec3 n, glm::vec3 m, glm::vec3 v, float alpha) {
    float n_dot_v = glm::dot(n, v);
    float nom = ggxMasking(n, v, alpha) * ggxDistribution(n, m, alpha);
    float denom = glm::max(4.0f * n_dot_v, eps);
    float pdf = glm::max(nom / denom, eps);
    return pdf;
}

float microfacetBRDF(glm::vec3 n, glm::vec3 l, glm::vec3 v, float roughness) {
    float alpha = roughness * roughness;
    float n_dot_l = glm::dot(n, l);
    float n_dot_v = glm::dot(n, v);
    glm::vec3 m = glm::normalize(l + v);
    float D = ggxDistribution(n, m, alpha);
    float G = ggxGeometry(n, l, v, alpha);
    float nom = G * D;
    float denom = glm::max(4.0f * n_dot_l * n_dot_v, eps);
    return glm::min(nom / denom, 1.0f);
}

glm::vec3 lambertDiffuse(glm::vec3 diffuse) {
    return diffuse / glm::pi<float>();
}
}  // namespace

// https://schuttejoe.github.io/post/ggximportancesamplingpart1/
// https://agraphicsguy.wordpress.com/2015/11/01/sampling-microfacet-brdf/
GGXSample ggxImportanceSample(glm::vec3 normal, glm::vec3 view, float roughness) {
    float alpha = roughness * roughness;
    glm::vec3 halfway = ggxGenerateNormal(normal, alpha, canonicalRandom(), canonicalRandom());
    float pdf = ggxPDF(normal, halfway, view, alpha);
    return {.halfway = halfway, .pdf = pdf};
}

// https://schuttejoe.github.io/post/ggximportancesamplingpart2/
// https://hal.archives-ouvertes.fr/hal-01509746/document
// http://jcgt.org/published/0007/04/01/paper.pdf
GGXSample ggxVisibilityImportanceSample(glm::vec3 normal, glm::vec3 view, float roughness) {
    float alpha = roughness * roughness;
    glm::vec3 halfway = ggxVisibilityGenerateNormal(normal, view, alpha, canonicalRandom(), canonicalRandom());
    float pdf = ggxVisibilityPDF(normal, halfway, view, alpha);
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
