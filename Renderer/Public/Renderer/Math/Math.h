#pragma once

#include "detail/vector.h"
#include "detail/quaternion.h"
#include "detail/matrix.h"

namespace Math
{
    using Float2 = math::detail::vector2<float>;
    using Float3 = math::detail::vector3<float>;
    using Float4 = math::detail::vector4<float>;
    
    using Float4x4 = math::detail::matrix4x4<float>;
    
    using Quaternion = math::detail::quaternion<float>;

    template<typename T>
    math::detail::matrix4x4<T> MakePerspectiveProjection(float fovY, float aspect, float near, float far)
    {
        math::detail::matrix4x4<T> m;
        const float tanHalfFovY = std::tan(fovY * 0.5f);

        m.data.columns[0][0] = 1.0f / (tanHalfFovY * aspect);
        m.data.columns[1][1] = 1.0f / tanHalfFovY;
        m.data.columns[2][2] = far / (near - far);
        m.data.columns[2][3] = -1.0f;
        m.data.columns[3][2] = far / (near - far) * near;
        m.data.columns[3][3] = 0.0f;

        return m;
    }

    template<typename T>
    math::detail::matrix4x4<T> MakeOrtographicProjection(float left, float right, float bottom, float top, float nearZ, float farZ)
    {
        math::detail::matrix4x4<T> m;
        m.data.columns[0] = simd::float4{ 2 / (right - left), 0, 0, 0 };
        m.data.columns[1] = simd::float4{ 0, 2 / (top - bottom), 0, 0 };
        m.data.columns[2] = simd::float4{ 0, 0, 1 / (farZ - nearZ), 0 };
        m.data.columns[3] = simd::float4{ (left + right) / (left - right), (top + bottom) / (bottom - top), nearZ / (nearZ - farZ), 1 };
        return m;
    }

    template<typename T>
    inline CONSTEXPR_QUALIFIER math::detail::matrix4x4<T> translate(const math::detail::vector3<T>& position)
    {
        math::detail::matrix4x4<T> m;
        math::detail::matrix4x4_make_translation<T>::execute(m, position);
        return m;
    }
}
