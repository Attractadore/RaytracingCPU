#pragma once

#include <memory>
#include <string>

class DynamicLibrary {
public:
    DynamicLibrary(const std::string& name, const std::string& path = ".");
    ~DynamicLibrary() noexcept;
    DynamicLibrary(const DynamicLibrary& other) = delete;
    DynamicLibrary(DynamicLibrary&& other) noexcept;
    DynamicLibrary& operator=(const DynamicLibrary& other) = delete;
    DynamicLibrary& operator=(DynamicLibrary&& other) noexcept;

    template <typename FunctionOrPtr>
    auto getFunction(const std::string& function_name) noexcept {
        using Function = std::remove_pointer_t<FunctionOrPtr>;
        static_assert(std::is_function_v<Function>);
        return reinterpret_cast<Function*>(getFunctionBase(function_name));
    }

    explicit operator bool() const noexcept;

private:
    class Impl;
    std::unique_ptr<Impl> pimpl;

    void* getFunctionBase(const std::string& function_name) noexcept;
};
