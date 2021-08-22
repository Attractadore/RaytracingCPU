#pragma once
#include "Material.hpp"

#include <memory>

template <typename MaterialSubclass>
const Material* getMaterialSubclass() {
    static_assert(std::is_base_of_v<Material, MaterialSubclass>);
    static MaterialSubclass material;
    return &material;
}

extern "C" {
const Material* getMaterial();
using GetMaterialF = decltype(getMaterial);
}
