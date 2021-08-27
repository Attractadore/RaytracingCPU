#include <boost/predef.h>

#if BOOST_OS_WINDOWS
#include "WindowsDynamicLoader.cpp"
#elif BOOST_OS_UNIX
#include "UnixDynamicLoader.cpp"
#else
#error Unknown platform
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
