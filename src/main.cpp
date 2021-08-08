#include "Intersectable.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "MaterialFactory.hpp"
#include "Scene.hpp"
#include "Util/Color.hpp"
#include "Util/Math.hpp"

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <atomic>
#include <execution>
#include <iostream>

std::vector<Ray> generateScreenRays(unsigned width, unsigned height, glm::mat4 proj_view_inv, glm::vec3 origin) {
    std::vector<Ray> rays;
    rays.reserve(width * height);
    for (unsigned y = 0; y < height; y++) {
        float f_y = linearRemap(0, height - 1, 1.0f, -1.0f, y);
        for (unsigned x = 0; x < width; x++) {
            float f_x = linearRemap(0, width - 1, -1.0f, 1.0f, x);
            glm::vec4 world = proj_view_inv * glm::vec4{f_x, f_y, 0.0f, 1.0f};
            glm::vec3 direction = glm::normalize(glm::vec3{world / world.w} - origin);
            rays.emplace_back(origin, direction, 3);
        }
    }
    return rays;
}

Uint32 mapRGB(glm::vec3 pixel, const SDL_PixelFormat* pixel_format) {
    pixel = linearToSrgb(pixel);
    pixel = glm::clamp(pixel, 0.0f, 1.0f);
    glm::uvec3 upixel = float(std::numeric_limits<Uint8>::max()) * pixel;
    return SDL_MapRGB(pixel_format, upixel.r, upixel.g, upixel.b);
}

void setSurfacePixels(SDL_Surface* surface, const std::vector<glm::vec3>& pixels) {
    assert(pixels.size() == surface->w * surface->h);
    Uint32* surface_pixels = static_cast<Uint32*>(surface->pixels);
    glm::vec3 average = reinhardAverage(pixels);
    for (unsigned i = 0; i < surface->w * surface->h; i++) {
        surface_pixels[i] = mapRGB(reinhardToneMap(pixels[i], average), surface->format);
    }
}

int main() {
    int width = 1280;
    int height = 720;

    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("RayTracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

    auto blue = makeMaterial("BlueMaterial");
    auto mirror = makeMaterial("MirrorMaterial");
    auto marble = makeMaterial("MarbleMaterial");
    auto grey = makeMaterial("GreyMaterial");
    auto gold = makeMaterial("GoldMaterial");

    Sphere marble_sphere{glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 1.0f})};
    MaterialIntersectable marble_material_sphere{
        .object = &marble_sphere,
        .material = marble.get(),
    };

    Sphere gold_sphere{glm::translate(glm::mat4(1.0f), {-3.0f, 2.0f, 1.0f})};
    MaterialIntersectable gold_material_sphere{
        .object = &gold_sphere,
        .material = gold.get(),
    };

    Sphere mirror_sphere{glm::translate(glm::mat4(1.0f), {4.0f, -3.0f, 1.0f})};
    MaterialIntersectable mirror_material_sphere{
        .object = &mirror_sphere,
        .material = mirror.get(),
    };

    Sphere blue_sphere{glm::translate(glm::mat4(1.0f), {-2.0f, -3.0f, 1.0f})};
    MaterialIntersectable blue_material_sphere{
        .object = &blue_sphere,
        .material = blue.get(),
    };

    Plane grey_plane{};

    MaterialIntersectable grey_material_plane{
        .object = &grey_plane,
        .material = grey.get(),
    };

    DirectionalLight sun{glm::vec3{10.0f, 10.0f, 9.0f},
                         glm::normalize(glm::vec3{1.0f, 0.0f, -1.0f})};

    Scene scene = {
        .objects = {&marble_material_sphere, &gold_material_sphere, &mirror_material_sphere, &blue_material_sphere, &grey_material_plane},
        .lights = {&sun},
        .camera = {
            .position{-10.0f, 0.0f, 5.0f},
            .direction = glm::normalize(glm::vec3{10.0f, 0.0f, -5.0f}),
            .up{0.0f, 0.0f, 1.0f},
        },
        .eps = 0.001f,
    };

    glm::mat4 view = cameraView(scene.camera);

    float hfov = 45.0f;
    float aspect = float(width) / float(height);
    glm::mat4 proj = glm::infinitePerspective(glm::radians(hfov), aspect, scene.eps);

    glm::mat4 proj_view_inv = glm::inverse(proj * view);

    unsigned num_acc_samples = 0;
    unsigned num_samples = 16;
    std::vector<glm::vec3> accumulated_colors(width * height, glm::vec3{0.0f});
    std::vector<glm::vec3> pixels(width * height);
    auto policy = std::execution::par_unseq;

    bool exit = false;
    while (!exit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
                exit = true;
            }
        }

        auto rays = generateScreenRays(width, height, proj_view_inv, scene.camera.position);
        std::transform(policy, rays.begin(), rays.end(), accumulated_colors.begin(), accumulated_colors.begin(), [&](Ray r, glm::vec3 color) {
            for (unsigned i = 0; i < num_samples; i++) {
                color += sceneIntersectColor(scene, r);
            }
            return color;
        });
        num_acc_samples += num_samples;

        std::transform(policy, accumulated_colors.begin(), accumulated_colors.end(), pixels.begin(), [=](glm::vec3 accumulated_color) {
            return accumulated_color / float(num_acc_samples);
        });

        SDL_Surface* window_surface = SDL_GetWindowSurface(window);
        setSurfacePixels(window_surface, pixels);
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}
