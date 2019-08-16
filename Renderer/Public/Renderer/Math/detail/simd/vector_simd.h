#pragma once

#include "simd_types.h"

namespace math::detail
{
    template<>
    struct compute_vector2_add<float>
    {
        static vector2<float> execute(const vector2<float>& a, const vector2<float>& b)
        {
            vector2<float> result;
            result.data = a.data + b.data;
            return result;
        }
    };
    
    template<>
    struct vector2_init<float>
    {
        static void execute(float x, float y, vector2<float>& q)
        {
            q.data = simd::float2{ x, y };
        }
    };
    
    template<>
    struct vector3_init<float>
    {
        static void execute(float x, float y, float z, vector3<float>& q)
        {
            q.data = simd::float3{ x, y, z};
        }
    };
    
    template<>
    struct vector4_init<float>
    {
        static void execute(float x, float y, float z, float w, vector4<float>& q)
        {
            q.data = simd::float4{ x, y, z, w};
        }
    };
}
