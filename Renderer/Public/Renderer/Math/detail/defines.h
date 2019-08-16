#pragma once

#include <type_traits>

#if defined(CONSTEXPR_ALLOWED)
#define CONSTEXPR_QUALIFIER constexpr
#else
#define CONSTEXPR_QUALIFIER
#endif

template<typename T>
struct is_float : std::false_type
{};

template<>
struct is_float<float> : std::true_type
{};

namespace math::detail
{
    template<uint8_t L, typename T>
    struct storage : std::false_type
    {
    };
    
    template<uint8_t L, typename T>
    using storage_t = typename storage<L,T>::type;
}
