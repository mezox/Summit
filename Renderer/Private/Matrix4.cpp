#include <Renderer/Matrix4.h>

#include <simd/matrix_types.h>
#include <simd/matrix.h>

#include <iostream>
#include <iomanip>

#include <cmath>

namespace detail
{
    template<typename T>
    T& CastToSimd(void* data)
    {
        return *reinterpret_cast<T*>(data);
    }
    
//    template<typename T>
//    T& CastToSimd(std::aligned_storage<64, 16>& data)
//    {
//        return *reinterpret_cast<T*>((void*)data);
//    }
}

Float4x4::Float4x4()
{
    new (&mStorage) simd::float4x4(std::move(matrix_identity_float4x4));
}

Float4x4::~Float4x4()
{}

Float4x4::Float4x4(float x, float y, float z, float w)
{
    float xx = x * x;
    float xy = x * y;
    float xz = x * z;
    float xw = x * w;
    float yy = y * y;
    float yz = y * z;
    float yw = y * w;
    float zz = z * z;
    float zw = z * w;
    
    // indices are m<column><row>
    float m00 = 1 - 2 * (yy + zz);
    float m10 = 2 * (xy - zw);
    float m20 = 2 * (xz + yw);
    
    float m01 = 2 * (xy + zw);
    float m11 = 1 - 2 * (xx + zz);
    float m21 = 2 * (yz - xw);
    
    float m02 = 2 * (xz - yw);
    float m12 = 2 * (yz + xw);
    float m22 = 1 - 2 * (xx + yy);
    
    
    matrix_float4x4 matrix = matrix_identity_float4x4;
    matrix.columns[0] = simd::float4{ m00, m10, m20, 0 };
    matrix.columns[1] = simd::float4{ m01, m11, m21, 0 };
    matrix.columns[2] = simd::float4{ m02, m12, m22, 0 };
    matrix.columns[3] = simd::float4{   0,   0,   0, 1};
    
    new (&mStorage) simd::float4x4(std::move(matrix));
}

Float4x4& Float4x4::operator*=(const Float4x4& other)
{
    simd::float4x4& rotationMatrix = detail::CastToSimd<simd::float4x4>((void*)&other.mStorage);
    simd::float4x4& matrix = detail::CastToSimd<simd::float4x4>((void*)&mStorage);
    
    matrix *= rotationMatrix;
    return *this;
}

Float4x4 Float4x4::operator*(const Float4x4& other)
{
    Float4x4 out;
    
    simd::float4x4& outMatrix = detail::CastToSimd<simd::float4x4>((void*)&out.mStorage);
    simd::float4x4& rotationMatrix = detail::CastToSimd<simd::float4x4>((void*)&other.mStorage);
    simd::float4x4& matrix = detail::CastToSimd<simd::float4x4>((void*)&mStorage);
    
    outMatrix = matrix_multiply(matrix, rotationMatrix);
    return out;
}

void Float4x4::MakeIdentity()
{
    simd::float4x4& matrix = detail::CastToSimd<simd::float4x4>((void*)&mStorage);
    matrix = std::move(matrix_identity_float4x4);
}

void Float4x4::SetTranslation(float x, float y, float z)
{
    auto& matrix = detail::CastToSimd<simd::float4x4>((void*)&mStorage);
    
    matrix.columns[3].x = x;
    matrix.columns[3].y = y;
    matrix.columns[3].z = z;
}

//void Float4x4::RotateY(float angle)
//{
//    Float4x4 m;
//    simd::float4x4& rotationMatrix = detail::CastToSimd<simd::float4x4>((void*)&m.mStorage);
//    simd::float4x4& matrix = detail::CastToSimd<simd::float4x4>((void*)&mStorage);
//    
//    const float cosAngle = std::cos(angle);
//    const float sinAngle = std::sin(angle);
//    
//    rotationMatrix.columns[0][0] = cosAngle;
//    rotationMatrix.columns[0][2] = sinAngle;
//    rotationMatrix.columns[2][0] = -sinAngle;
//    rotationMatrix.columns[2][2] = cosAngle;
//    
//    matrix *= rotationMatrix;
//}
//
//void Float4x4::RotateX(float angle)
//{
//    Float4x4 m;
//    simd::float4x4& rotationMatrix = detail::CastToSimd<simd::float4x4>((void*)&m.mStorage);
//    simd::float4x4& matrix = detail::CastToSimd<simd::float4x4>((void*)&mStorage);
//    
//    const float cosAngle = std::cos(angle);
//    const float sinAngle = std::sin(angle);
//    
//    rotationMatrix.columns[1][1] = cosAngle;
//    rotationMatrix.columns[1][2] = sinAngle;
//    rotationMatrix.columns[2][1] = -sinAngle;
//    rotationMatrix.columns[2][2] = cosAngle;
//    
//    matrix = matrix_multiply(matrix, rotationMatrix);
//}
//
//void Float4x4::RotateZ(float angle)
//{
//    Rotate(angle, [](Float4x4& m, float cos, float sin) {
//        
//        simd::float4x4& simdMat = detail::CastToSimd<simd::float4x4>((void*)&m.mStorage);
//        
//        simdMat.columns[0][0] = cos;
//        simdMat.columns[0][1] = sin;
//        simdMat.columns[1][0] = -sin;
//        simdMat.columns[1][1] = cos;
//    });
//}

void Float4x4::Rotate(float angle, RotationHandler handler)
{
    Float4x4 m;
    simd::float4x4& rotationMatrix = detail::CastToSimd<simd::float4x4>((void*)&m.mStorage);
    simd::float4x4& matrix = detail::CastToSimd<simd::float4x4>((void*)&mStorage);
    
    const float cosAngle = std::cos(angle);
    const float sinAngle = std::sin(angle);
    
    handler(m, cosAngle, sinAngle);
    
    matrix = matrix_multiply(matrix, rotationMatrix);
}

Float4x4& Float4x4::Inverse()
{
    simd::float4x4& matrix = detail::CastToSimd<simd::float4x4>((void*)&mStorage);
    matrix = simd::inverse(matrix);
    return *this;
}

Float4x4 Float4x4::GetInverse() const
{
    Float4x4 m;
    simd::float4x4& result = detail::CastToSimd<simd::float4x4>((void*)&m.mStorage);
    simd::float4x4& matrix = detail::CastToSimd<simd::float4x4>((void*)&mStorage);
    
    result = simd::inverse(matrix);
    return m;
}

std::ostream& operator<<(std::ostream& s, const Float4x4& m)
{
    const auto& matrix = detail::CastToSimd<const simd::float4x4>((void*)&m.mStorage);

    for(uint8_t i{ 0 } ; i < 4; ++i)
    {
        for(uint8_t j{ 0 } ; j < 4; ++j)
        {
            s << std::right << std::fixed << std::setw(7) << std::setprecision(3) << matrix.columns[j][i];
        }

        s << "\n";
    }
    
    return s;
}

Float4x4 Float4x4::MakePerspective(float fovY, float aspect, float near, float far)
{
    Float4x4 m;
    auto& matrix = detail::CastToSimd<simd::float4x4>((void*)&m.mStorage);
    
    const float tanHalfFovY = std::tan(fovY * 0.5f);

    matrix.columns[0][0] = 1.0f / (tanHalfFovY * aspect);
    matrix.columns[1][1] = 1.0f / tanHalfFovY;
    matrix.columns[2][2] = far / (near - far);
    matrix.columns[2][3] = -1.0f;
    matrix.columns[3][2] = far / (near - far) * near;
    matrix.columns[3][3] = 0.0f;
    
    return m;
}

