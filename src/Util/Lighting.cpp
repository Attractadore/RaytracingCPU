#include "Lighting.hpp"

#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <random>

glm::vec3 blinnPhong(
    glm::vec3 incident,
    glm::vec3 light_dir,
    float light_intensity,
    glm::vec3 light_color,
    glm::vec3 diffuse,
    float specular_hardness,
    float diffuse_weight,
    float specular_weight,
    glm::vec3 normal) {
    float total_weight = diffuse_weight + specular_weight;
    diffuse_weight /= total_weight;
    specular_weight /= total_weight;
    glm::vec3 halfway_dir = glm::normalize(light_dir - incident);
    float diffuse_strength = diffuse_weight * glm::max(glm::dot(normal, light_dir), 0.0f);
    float specular_strength = specular_weight * glm::pow(glm::max(glm::dot(normal, halfway_dir), 0.0f), specular_hardness);
    return light_intensity * (diffuse_strength * diffuse + specular_strength) * light_color;
}

glm::vec3 blinnPhong(glm::vec3 normal, glm::vec3 light_dir, glm::vec3 view_dir, glm::vec3 diffuse, glm::vec3 specular, float specular_hardness) {
    glm::vec3 halfway_dir = glm::normalize(light_dir + view_dir);
    float n_dot_l = glm::max(glm::dot(normal, light_dir), 0.0f);
    float n_dot_h = glm::max(glm::dot(normal, halfway_dir), 0.0f);
    float kd = n_dot_l;
    float ks = glm::pow(n_dot_h, specular_hardness);
    return kd * diffuse + ks * specular;
}

namespace {
std::random_device rd;
std::mt19937 gen{rd()};
std::uniform_real_distribution dis{0.0f, 1.0f};

float posChar(float x) {
    return x > 0.0f;
}

float ggxDistribution(glm::vec3 n, glm::vec3 m, float a) {
    float n_dot_m = glm::dot(n, m);
    float n_dot_m2 = n_dot_m * n_dot_m;
    float a2 = a * a;
    float nom = posChar(n_dot_m) * a2;
    float denom = 1.0f + n_dot_m2 * (a2 - 1.0f); 
    denom = glm::pi<float>() * denom * denom;
    return nom / denom;
}

float ggxPDF(float cos_theta, float cos_theta2, float sin_theta, float a2) {
    float nom = a2 * cos_theta;
    float denom = 1.0f + cos_theta2 * (a2 - 1.0f);
    denom = glm::pi<float>() * denom * denom;
    return nom / denom;
}

glm::vec3 frenel(glm::vec3 n, glm::vec3 l, glm::vec3 f0) {
    float n_dot_l = glm::max(glm::dot(n, l), 0.0f);
    return f0 + (1.0f - f0) * glm::pow(1 - n_dot_l, 5.0f);
}

float f0Dielectric(float eta) {
    float f0 = (eta - 1.0f) / (eta + 1.0f);
    return f0 * f0;
}

glm::vec3 f0Metal(glm::vec3 diffuse) {
    return diffuse;
}

glm::vec3 frenel(glm::vec3 n, glm::vec3 l, float eta, glm::vec3 diffuse, float metallic) {
    return frenel(n, l, glm::mix(glm::vec3{f0Dielectric(eta)}, f0Metal(diffuse), metallic));
}

float microfacetBRDF(glm::vec3 n, glm::vec3 l, glm::vec3 v, float roughness) {
    float a = roughness * roughness;
    glm::vec3 h = glm::normalize(l + v);
    float n_dot_l = glm::max(glm::dot(n, l), 0.0f);
    float n_dot_v = glm::max(glm::dot(n, v), 0.0f);
    float D = ggxDistribution(n, h, a);
    float G = 0.5f / glm::max(glm::mix(2.0f * n_dot_l * n_dot_v, n_dot_l + n_dot_v, a), 0.01f);
    assert(!glm::isnan(G * D));
    return glm::min(G * D, 1.0f);
}

glm::vec3 lambertDiffuse(glm::vec3 diffuse) {
    return diffuse / glm::pi<float>();
}
}

GGXSample ggxImportanceSample(glm::vec3 normal, glm::vec3 view, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float rand_theta = dis(gen);
    float rand_phi = dis(gen);
    float cos_theta2 = (1.0f - rand_theta) / (rand_theta * (a2 - 1.0f) + 1.0f);
    float sin_theta2 = 1.0f - cos_theta2;
    float cos_theta = glm::sqrt(cos_theta2);
    float sin_theta = glm::sqrt(sin_theta2);
    float pdf = ggxPDF(cos_theta, cos_theta2, sin_theta, a2);
    float phi = glm::two_pi<float>() * rand_phi;
    float sin_phi = glm::sin(phi);
    float cos_phi = glm::cos(phi);
    glm::vec3 direction{sin_theta * cos_phi, sin_theta * sin_phi, cos_theta};
    glm::vec3 default_normal{0.0f, 0.0f, 1.0f};
    if (normal != default_normal) {
        glm::vec3 rotate_axis = glm::normalize(glm::cross(default_normal, normal));
        float rotate_angle = glm::acos(glm::dot(default_normal, normal));
        glm::mat3 rotate = glm::rotate(glm::mat4{1.0f}, rotate_angle, rotate_axis);
        direction = rotate * direction;
    }
    pdf = glm::max(pdf / (4.0f * glm::dot(view, direction)), 0.01f);
    return {.normal = direction, .pdf = pdf};
}

glm::vec3 cookTorrance(glm::vec3 normal, glm::vec3 light_dir, glm::vec3 view_dir, glm::vec3 diffuse, float roughness, float metallic, float eta, float solid_angle) {
    glm::vec3 ks = frenel(normal, light_dir, eta, diffuse, metallic);
    glm::vec3 kd = 1.0f - ks;
    kd *= (1.0f - metallic);
    float n_dot_l = glm::max(glm::dot(normal, light_dir), 0.0f);
    glm::vec3 diffuse_term = lambertDiffuse(diffuse);
    float specular_term = microfacetBRDF(normal, light_dir, view_dir, roughness);
    return (kd * diffuse_term + ks * specular_term) * n_dot_l * solid_angle;
}

glm::vec3 cookTorranceAbstractLight(glm::vec3 normal, glm::vec3 light_dir, glm::vec3 view_dir, glm::vec3 diffuse, float roughness, float metallic, float eta) {
    return cookTorrance(normal, light_dir, view_dir, diffuse, roughness, metallic, eta, glm::pi<float>());
}
