#pragma once
#include "Color.hpp"
#include "SDLHandle.hpp"
#include "Window.hpp"

#include <SDL2/SDL.h>

#include <limits>

class Window::Impl {
public:
    Impl(unsigned width, unsigned height);
    ~Impl() noexcept;
    Impl(const Impl& other) = delete;
    Impl(Impl&& other) noexcept;
    Impl& operator=(const Impl& other) = delete;
    Impl& operator=(Impl&& other) noexcept;

    Dimensions dimensions() const noexcept;

    void hide() noexcept;
    void setTitle(const std::string& title) noexcept;
    void setPixel(unsigned x, unsigned y, glm::vec3 pixel) noexcept;
    void setPixels(const std::vector<glm::vec3>& pixels) noexcept;
    void update() noexcept;
    bool shouldClose() noexcept;

private:
    SDLHandle handle;
    SDL_Window* window;
};

namespace {
Uint32 mapRGB(glm::vec3 pixel, const SDL_PixelFormat* format) {
    pixel = linearToSrgb(pixel);
    pixel = glm::clamp(pixel, 0.0f, 1.0f);
    glm::uvec3 upixel = float(std::numeric_limits<Uint8>::max()) * pixel;
    return SDL_MapRGB(format, upixel.r, upixel.g, upixel.b);
}
}  // namespace

Window::Impl::Impl(unsigned width, unsigned height):
    handle{} {
    window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    if (!window) {
        throw std::runtime_error("Failed to create SDL window");
    }
}

Window::Impl::~Impl() noexcept {
    SDL_DestroyWindow(window);
}

Window::Impl::Impl(Impl&& other) noexcept:
    window{std::exchange(other.window, nullptr)} {
}

Window::Impl& Window::Impl::operator=(Impl&& other) noexcept {
    SDL_DestroyWindow(window);
    window = std::exchange(other.window, nullptr);
    return *this;
}

Window::Dimensions Window::Impl::dimensions() const noexcept {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return {w, h};
}

void Window::Impl::hide() noexcept {
    SDL_HideWindow(window);
}

void Window::Impl::setTitle(const std::string& title) noexcept {
    SDL_SetWindowTitle(window, title.c_str());
}

void Window::Impl::setPixel(unsigned x, unsigned y, glm::vec3 pixel) noexcept {
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    assert(surface->format->BytesPerPixel == sizeof(Uint32));
    std::byte* data = static_cast<std::byte*>(surface->pixels);
    Uint32* pixel_row = reinterpret_cast<Uint32*>(data + surface->pitch * y);
    pixel_row[x] = mapRGB(pixel, surface->format);
}

void Window::Impl::setPixels(const std::vector<glm::vec3>& pixels) noexcept {
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    assert(surface->format->BytesPerPixel == sizeof(Uint32));
    auto map = [&](glm::vec3 pixel) {
        return mapRGB(pixel, surface->format);
    };
    if (surface->pitch == surface->w * sizeof(Uint32)) {
        auto surface_pixels = static_cast<Uint32*>(surface->pixels);
        std::transform(pixels.begin(), pixels.end(), surface_pixels, map);
    } else {
        auto surface_pixels = static_cast<std::byte*>(surface->pixels);
        for (unsigned y = 0; y < surface->h; y++) {
            auto pixel_row_first = pixels.cbegin() + surface->w * y;
            auto pixel_row_last = pixel_row_first + surface->w;
            Uint32* surface_pixel_row = reinterpret_cast<Uint32*>(surface_pixels + surface->pitch * y);
            std::transform(pixel_row_first, pixel_row_last, surface_pixel_row, map);
        }
    }
}

void Window::Impl::update() noexcept {
    SDL_UpdateWindowSurface(window);
}

bool Window::Impl::shouldClose() noexcept {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_WINDOWEVENT and
            e.window.windowID == SDL_GetWindowID(window) and
            e.window.event == SDL_WINDOWEVENT_CLOSE) {
            return true;
        }
    }
    return false;
}
