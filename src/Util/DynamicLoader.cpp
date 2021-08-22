#include "DynamicLoader.hpp"

#include <boost/predef.h>

#if BOOST_OS_WINDOWS
#include <Windows.h>
#elif BOOST_OS_MACOS || BOOST_OS_LINUX
#include <dlfcn.h>
#else
#error Unknown platform
#endif

namespace {
#if BOOST_OS_WINDOWS
std::string libFilename(const std::string& name, const std::string& path) {
    return path + "/" + name + ".dll";
}
#elif BOOST_OS_MACOS
std::string libFilename(const std::string& name, const std::string& path) {
    return path + "/lib" + name + ".dylib";
}
#elif BOOST_OS_LINUX
std::string libFilename(const std::string& name, const std::string& path) {
    return path + "/lib" + name + ".so";
}
#endif
}

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
#if BOOST_OS_WINDOWS
    HMODULE handle;
#elif BOOST_OS_MACOS || BOOST_OS_LINUX
    void* handle;
#endif
};

#if BOOST_OS_WINDOWS
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
#elif BOOST_OS_MACOS || BOOST_OS_LINUX
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
#endif

DynamicLibrary::DynamicLibrary(const std::string& name, const std::string& path) {
    pimpl = std::make_unique<Impl>(name, path);
}

DynamicLibrary::~DynamicLibrary() noexcept = default;
DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) noexcept = default;
DynamicLibrary& DynamicLibrary::operator=(DynamicLibrary&& other) noexcept = default;

DynamicLibrary::operator bool() const noexcept {
    return bool(*pimpl);
}

void* DynamicLibrary::getFunctionBase(const std::string& function_name) noexcept {
    return pimpl->getFunction(function_name);
}
