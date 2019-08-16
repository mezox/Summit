#pragma once

#include "simd_types.h"

#include <simd/matrix.h>

namespace math::detail
{
    template<>
    struct matrix4x4_init<float>
    {
        static void execute(matrix4x4<float>& m)
        {
            m.data = matrix_identity_float4x4;
        }
    };

    template<>
    struct matrix4x4_init_quaternion<float>
    {
        static void execute(matrix4x4<float>& m, const quaternion<float>& q)
        {
            const float xx = q.x * q.x;
            const float xy = q.x * q.y;
            const float xz = q.x * q.z;
            const float xw = q.x * q.w;
            const float yy = q.y * q.y;
            const float yz = q.y * q.z;
            const float yw = q.y * q.w;
            const float zz = q.z * q.z;
            const float zw = q.z * q.w;

            // indices are m<column><row>
            const float m00 = 1 - 2 * (yy + zz);
            const float m10 = 2 * (xy - zw);
            const float m20 = 2 * (xz + yw);

            const float m01 = 2 * (xy + zw);
            const float m11 = 1 - 2 * (xx + zz);
            const float m21 = 2 * (yz - xw);

            const float m02 = 2 * (xz - yw);
            const float m12 = 2 * (yz + xw);
            const float m22 = 1 - 2 * (xx + yy);

            m.data.columns[0] = simd::float4{ m00, m10, m20, 0 };
            m.data.columns[1] = simd::float4{ m01, m11, m21, 0 };
            m.data.columns[2] = simd::float4{ m02, m12, m22, 0 };
            m.data.columns[3] = simd::float4{   0,   0,   0, 1 };
        }
    };

    template<>
    struct matrix4x4_compute_inverse<float>
    {
        static matrix4x4<float> execute(const matrix4x4<float>& m)
        {
            matrix4x4<float> result;
            result.data = simd::inverse(m.data);
            return result;
        }
    };

    template<>
    struct matrix4x4_make_translation<float> : std::true_type
    {
        static void execute(matrix4x4<float>& m, const vector3<float>& v)
        {
            m.data.columns[3].x = v.data.x;
            m.data.columns[3].y = v.data.y;
            m.data.columns[3].z = v.data.z;
        }
    };

    template<>
    struct matrix4x4_multiply<float> : std::true_type
    {
        static matrix4x4<float> execute(const matrix4x4<float>& a, const matrix4x4<float>& b)
        {
            matrix4x4<float> result;
            result.data = matrix_multiply(a.data, b.data);
            return result;
        }
    };
}
