#include "RefractMaterialTemplate.cpp"

using LessDense = RefractiveMaterialTemplate<.7f>;

std::unique_ptr<Material> makeMaterial() {
    return std::make_unique<LessDense>();
}
