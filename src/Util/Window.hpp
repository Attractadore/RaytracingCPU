#pragma once
#include "PixelIterator.hpp"

#include <glm/mat4x4.hpp>

#include <memory>
#include <string>
#include <vector>

class Window {
public:
    Window(unsigned width, unsigned height);
    ~Window() noexcept;
    Window(const Window& other) = delete;
    Window(Window&& other) noexcept;
    Window& operator=(const Window& other) = delete;
    Window& operator=(Window&& other) noexcept;

    struct Dimensions {
        unsigned width, height;
    };
    Dimensions dimensions() const noexcept;
    void hide() noexcept;
    void setTitle(const std::string& title) noexcept;
    void setPixel(unsigned x, unsigned y, glm::vec3 pixel) noexcept;
    void setPixels(const std::vector<glm::vec3>& pixels) noexcept;
    template <PixelForwardIterator I>
    void setPixels(I first, I last) {
        setPixels({first, last});
    }
    void update() noexcept;
    bool shouldClose() noexcept;

    float hfov = 45.0f;
    unsigned samples = 1;

private:
    class Impl;
    std::unique_ptr<Impl> pimpl;
};

float windowAspectRatio(const Window& window) noexcept;
glm::mat4 windowPerspective(const Window& window, float eps) noexcept;
