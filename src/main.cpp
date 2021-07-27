#include "Intersectable.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "MaterialFactory.hpp"
#include "Scene.hpp"
#include "Util/Math.hpp"
#include "Util/Color.hpp"

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

std::vector<Ray> generateScreenRays(unsigned width, unsigned height, glm::mat4 proj_view_inv, glm::vec3 origin) {
    std::vector<Ray> rays;
    rays.reserve(width * height);
    for (unsigned y = 0; y < height; y++) {
        float f_y = linearRemap(0, height - 1, 1.0f, -1.0f, y);
        for (unsigned x = 0; x < width; x++) {
            float f_x = linearRemap(0, width - 1, -1.0f, 1.0f, x);
            glm::vec4 world = proj_view_inv * glm::vec4{f_x, f_y, 0.0f, 1.0f};
            glm::vec3 direction = glm::normalize(glm::vec3{world / world.w} - origin);
            rays.push_back({.origin = origin, .direction = direction});
        }
    }
    return rays;
}

Uint32 mapRGB(glm::vec3 pixel, SDL_PixelFormat const* pixel_format) {
    pixel = linearToSrgb(pixel);
    pixel = glm::clamp(pixel, 0.0f, 1.0f);
    glm::uvec3 upixel = float(std::numeric_limits<Uint8>::max()) * pixel;
    return SDL_MapRGB(pixel_format, upixel.r, upixel.g, upixel.b);
}

void setSurfacePixels(SDL_Surface* surface, const std::vector<glm::vec3>& pixels) {
    assert(pixels.size() == surface->w * surface->h);
    Uint32* surface_pixels = (Uint32*) surface->pixels;
    for (unsigned i = 0; i < surface->w * surface->h; i++) {
        surface_pixels[i] = mapRGB(pixels[i], surface->format);
    }
}

int main() {
    int width = 1280;
    int height = 720;

    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("RayTracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    std::vector<glm::vec3> pixels(width * height);

    auto blue   = makeMaterial("BlueMaterial");
    auto mirror = makeMaterial("MirrorMaterial");
    auto marble = makeMaterial("MarbleMaterial");
    auto grey   = makeMaterial("GreyMaterial");

    Sphere blue_sphere{glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 1.0f})};
    MaterialIntersectable blue_material_sphere{
        .object = &blue_sphere,
        .material = blue.get(),
    };

    Sphere red_sphere{glm::translate(glm::mat4(1.0f), {-3.0f, 4.0f, 1.0f})};
    MaterialIntersectable red_material_sphere{
        .object = &red_sphere,
        .material = mirror.get(),
    };

    Sphere yellow_sphere{glm::translate(glm::mat4(1.0f), {4.0f, -3.0f, 1.0f})};
    MaterialIntersectable yellow_material_sphere{
        .object = &yellow_sphere,
        .material = marble.get(),
    };

    Plane grey_plane{};

    MaterialIntersectable grey_material_plane{
        .object = &grey_plane,
        .material = grey.get(),
    };

    DirectionalLight sun{{1.0f, 1.0f, 0.9f},
                         glm::normalize(glm::vec3{1.0f, 0.0f, -1.0f})};

    Scene scene = {
        .objects = {&blue_material_sphere, &red_material_sphere, &yellow_material_sphere, &grey_material_plane},
        .lights = {&sun},
        .camera = {
            .position{-10.0f, 0.0f, 2.0f},
            .direction{1.0f, 0.0f, 0.0f},
            .up{0.0f, 0.0f, 1.0f},
        },
        .eps = 0.001f,
    };

    glm::mat4 view = cameraView(scene.camera);

    float hfov = 45.0f;
    float aspect = float(width) / float(height);
    glm::mat4 proj = glm::infinitePerspective(glm::radians(hfov), aspect, scene.eps);

    glm::mat4 proj_view_inv = glm::inverse(proj * view);

    bool exit = false;
    while (!exit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
                exit = true;
            }
        }

        auto rays = generateScreenRays(width, height, proj_view_inv, scene.camera.position);
        std::transform(rays.begin(), rays.end(), pixels.begin(), [&](Ray r) {
            return sceneIntersectColor(scene, r);
        });

        SDL_Surface* window_surface = SDL_GetWindowSurface(window);
        setSurfacePixels(window_surface, pixels);
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}
