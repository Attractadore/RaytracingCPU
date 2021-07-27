#pragma once
#include "MaterialInterface.hpp"

#include <memory>

std::unique_ptr<Material> makeMaterial(const std::string& name);
