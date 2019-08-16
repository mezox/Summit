#pragma once

#include <type_traits>

template<typename F, typename T>
struct TypeLinkerImpl
{
    using from_t = F;
    using to_t = T;
    
    to_t operator()(const from_t& from);
};

template<typename F>
struct TypeLinkerAuto : public TypeLinkerImpl<std::false_type, std::false_type> {};

template<typename F, typename = typename TypeLinkerAuto<F>::from_t>
typename TypeLinkerAuto<F>::to_t to_metal_type(const F& from)
{
    return TypeLinkerAuto<F>{}(from);
}

template<typename T, typename F>
T to_metal_type(const F& from)
{
    return TypeLinkerImpl<F, T>{}(from);
}

// Forward declarations of C++ types
namespace MTL
{
    enum class StorageMode;
}

namespace Summit
{
    enum class MemoryType;
}

template<> struct TypeLinkerAuto<Summit::MemoryType> : public TypeLinkerImpl<Summit::MemoryType, MTL::StorageMode> {};
