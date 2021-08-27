#include "DynamicLoader.hpp"

#include <Windows.h>

namespace {
std::string libFilename(const std::string& name, const std::string& path) {
    return path + "/" + name + ".dll";
}
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
    HMODULE handle;
};

DynamicLibrary::Impl::Impl(const std::string& name, const std::string& path) {
    handle = LoadLibraryA(libFilename(name, path).c_str());
}

DynamicLibrary::Impl::~Impl() noexcept {
    if (handle) {
        FreeLibrary(handle);
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
    return handle ? GetProcAddress(handle, function_name.c_str()) : nullptr;
}
