#pragma once
#include "Material.hpp"

template <typename MaterialSubclass>
    requires std::is_base_of_v<Material, MaterialSubclass>
const Material* getMaterialSubclass() {
    static MaterialSubclass material;
    return &material;
}

extern "C" {
const Material* getMaterial();
using GetMaterialF = decltype(getMaterial);
}
