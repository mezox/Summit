#pragma once

#include <simd/vector_types.h>
#include <simd/matrix_types.h>

namespace math::detail
{
    template<>
    struct storage<4, float> : public std::true_type
    {
        using type = simd::float4;
    };
    
    template<>
    struct storage<3, float> : public std::true_type
    {
        using type = simd::float3;
    };
    
    template<>
    struct storage<2, float> : public std::true_type
    {
        using type = simd::float2;
    };
    
    template<>
    struct storage<16, float> : public std::true_type
    {
        using type = simd::float4x4;
    };
}
