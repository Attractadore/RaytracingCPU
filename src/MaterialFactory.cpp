#include "MaterialFactory.hpp"

#include <unordered_map>

#include "dlfcn.h"

struct MaterialMap {
    ~MaterialMap() {
        for (auto& [_, handle] : loaded) {
            dlclose(handle);
        }
    }
    std::unordered_map<std::string, void*> loaded;
};

MaterialMap materials;

static std::string libFileName(const std::string& name) {
    return "./lib" + name + ".so";
}

std::unique_ptr<Material> makeMaterial(const std::string& name) {
    auto res = materials.loaded.find(name);
    if (res == materials.loaded.end()) {
        auto handle = dlopen(libFileName(name).c_str(), RTLD_NOW);
        if (!handle) {
            std::fprintf(stderr, "Failed to load material: %s\n", dlerror());
            std::exit(-1);
        }
        res = materials.loaded.emplace(name, handle).first;
    }
    auto handle = res->second;
    auto mk = reinterpret_cast<MakeMaterialF>(dlsym(handle, "makeMaterial"));
    return mk();
}
