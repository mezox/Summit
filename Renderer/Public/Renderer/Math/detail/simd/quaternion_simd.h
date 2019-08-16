#pragma once

#include "simd_types.h"

#include <simd/geometry.h>

namespace math::detail
{
    template<>
    struct compute_quaternion_add<float>
    {
        static quaternion<float> execute(const quaternion<float>& a, const quaternion<float>& b)
        {
            quaternion<float> result;
            result.data = a.data + b.data;
            return result;
        }
    };
    
    template<>
    struct quaternion_init<float>
    {
        static void execute(float x, float y, float z, float w, quaternion<float>& q)
        {
            q.data = simd::float4{ x, y, z, w };
        }
    };
    
    template<typename U>
    struct compute_quaternion_length<float, U>
    {
        static U execute(const quaternion<float>& q)
        {
            return static_cast<U>(simd::length(q.data));
        }
    };
    
    template<typename U>
    struct compute_quaternion_length_squared<float, U>
    {
        static U execute(const quaternion<float>& q)
        {
            return static_cast<U>(simd_length_squared(q.data));
        }
    };
    
    template<>
    struct compute_quaternion_normalize<float>
    {
        static void execute(const quaternion<float>& q)
        {
            return simd::normalize(q.data);
        }
    };
}
