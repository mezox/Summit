#pragma once

#include <array>
#include <ostream>
#include <type_traits>
#include <functional>

class Float4x4 final
{
    using RotationHandler = std::function<void(Float4x4&, float, float)>;
    
public:
    Float4x4();
    ~Float4x4();
    
    Float4x4(float x, float y, float z, float w);
    
    Float4x4(const Float4x4& other) = default;
    Float4x4(Float4x4&& other) = default;
    
    Float4x4& operator=(const Float4x4& other) = default;
    Float4x4& operator=(Float4x4&& other) = default;
    
    Float4x4& operator*=(const Float4x4& other);
    Float4x4 operator*(const Float4x4& other);
    
    void MakeIdentity();
    void SetTranslation(float x, float y, float z);
    
    Float4x4& Inverse();
    Float4x4 GetInverse() const;
    
    friend std::ostream& operator<<(std::ostream& s, const Float4x4& m);
    
private:
    void Rotate(float angle, RotationHandler handler);
    
public:
    static Float4x4 MakePerspective(float fovY, float aspect, float near, float far);
    
private:
    std::aligned_storage<64, 16>::type mStorage;
};

