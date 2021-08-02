#pragma once
#include "Material.hpp"

#include <memory>

extern "C" {
using MakeMaterialF = std::unique_ptr<Material> (*)();
std::unique_ptr<Material> makeMaterial();
}
