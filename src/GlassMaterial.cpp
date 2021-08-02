#include "RefractMaterialTemplate.cpp"

using Glass = RefractiveMaterialTemplate<1.5f>;

std::unique_ptr<Material> makeMaterial() {
    return std::make_unique<Glass>();
}
