#include "SDLWindow.cpp"

#include <glm/gtc/matrix_transform.hpp>

Window::Window(unsigned width, unsigned height) {
    pimpl = std::make_unique<Window::Impl>(width, height);
}
Window::~Window() noexcept = default;
Window::Window(Window&& other) noexcept = default;
Window& Window::operator=(Window&& other) noexcept = default;

Window::Dimensions Window::dimensions() const noexcept {
    return pimpl->dimensions();
}

void Window::hide() noexcept {
    pimpl->hide();
}

void Window::setTitle(const std::string& title) noexcept {
    pimpl->setTitle(title);
}

void Window::setPixel(unsigned x, unsigned y, glm::vec3 pixel) noexcept {
    pimpl->setPixel(x, y, pixel);
}

void Window::setPixels(const std::vector<glm::vec3>& pixels) noexcept {
    pimpl->setPixels(pixels);
}

void Window::update() noexcept {
    pimpl->update();
}

bool Window::shouldClose() noexcept {
    return pimpl->shouldClose();
}

float windowAspectRatio(const Window& window) noexcept {
    auto [width, height] = window.dimensions();
    return float(width) / float(height);
}

glm::mat4 windowPerspective(const Window& window, float eps) noexcept {
    return glm::infinitePerspective(glm::radians(window.hfov), windowAspectRatio(window), eps);
}
