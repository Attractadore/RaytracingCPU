#pragma once
#include "Scene.hpp"

#include <string>

struct NamedScene {
    std::string name;
    Scene scene;

    explicit operator bool() {
        return name.size();
    }
};

NamedScene loadScene(const std::string& path);
