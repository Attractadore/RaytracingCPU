#include "MaterialLoader.hpp"
#include "Util/DynamicLoader.hpp"

#include <unordered_map>

Material* getMaterial(const std::string& name) {
    static std::unordered_map<std::string, DynamicLibrary> materials;
    auto kv = materials.find(name);
    if (kv == materials.end()) {
        auto dll = DynamicLibrary{name, "Materials"};
        if (!dll) {
            return nullptr;
        }
        kv = materials.emplace(name, std::move(dll)).first;
    }
    return kv->second.getFunction<GetMaterialF>("getMaterial")();
}
