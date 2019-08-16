#pragma once

#include <type_traits>

template<typename F, typename T>
struct TypeLinkerTempl
{
    using from_t = F;
    using to_t = T;
    
    to_t operator()(const from_t& from);
};

template<typename F>
struct TypeLinkerAuto : public TypeLinkerTempl<std::false_type, std::false_type> {};

template<typename F, typename = typename TypeLinkerAuto<F>::from_t>
typename TypeLinkerAuto<F>::to_t ConvertType(const F& from)
{
    return TypeLinkerAuto<F>{}(from);
}

template<typename T, typename F>
T ConvertType(const F& from)
{
    return TypeLinkerTempl<F, T>{}(from);
}
