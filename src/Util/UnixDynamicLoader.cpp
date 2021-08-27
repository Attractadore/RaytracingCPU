#include "DynamicLoader.hpp"

#include <boost/predef.h>

#include <dlfcn.h>

namespace {
#if BOOST_OS_MACOS
std::string libFilename(const std::string& name, const std::string& path) {
    return path + "/lib" + name + ".dylib";
}
#else
std::string libFilename(const std::string& name, const std::string& path) {
    return path + "/lib" + name + ".so";
}
#endif
}  // namespace

class DynamicLibrary::Impl {
public:
    Impl(const std::string& name, const std::string& path);
    ~Impl() noexcept;
    Impl(const Impl& other) = delete;
    Impl(Impl&& other) noexcept;
    Impl& operator=(const Impl& other) = delete;
    Impl& operator=(Impl&& other) noexcept;

    explicit operator bool() const noexcept;

    void* getFunction(const std::string& function_name) noexcept;

private:
    void* handle;
};

DynamicLibrary::Impl::Impl(const std::string& name, const std::string& path) {
    handle = dlopen(libFilename(name, path).c_str(), RTLD_NOW);
}

DynamicLibrary::Impl::~Impl() noexcept {
    if (handle) {
        dlclose(handle);
    }
}

DynamicLibrary::Impl::Impl(Impl&& other) noexcept {
    handle = std::exchange(other.handle, nullptr);
}

DynamicLibrary::Impl& DynamicLibrary::Impl::operator=(Impl&& other) noexcept {
    handle = std::exchange(other.handle, nullptr);
    return *this;
}

DynamicLibrary::Impl::operator bool() const noexcept {
    return handle;
}

void* DynamicLibrary::Impl::getFunction(const std::string& function_name) noexcept {
    return handle ? dlsym(handle, function_name.c_str()) : nullptr;
}
