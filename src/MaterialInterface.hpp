#pragma once
#include "Material.hpp"

#include <memory>

template <typename MaterialSubclass>
Material* getMaterialSubclass() {
    static_assert(std::is_base_of_v<Material, MaterialSubclass>);
    static MaterialSubclass material;
    return &material;
}

extern "C" {
using GetMaterialF = Material*();
Material* getMaterial();
static_assert(std::is_same_v<GetMaterialF, decltype(getMaterial)>);
}
