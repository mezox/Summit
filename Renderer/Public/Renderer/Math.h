#pragma once

#include <simd/simd.h>

#include <tuple>
#include <numeric>
#include <vector>
#include <functional>
#include <array>

#include "Math/Math.h"

namespace
{
    size_t hash_combiner(size_t left, size_t right)
    {
        return right + 0x9e3779b9 + (left << 6) + (left >> 2);
    }
    
    template<int index, class...types>
    struct hash_impl {
        size_t operator()(size_t a, const std::tuple<types...>& t) const {
            typedef typename std::tuple_element<index, std::tuple<types...>>::type nexttype;
            hash_impl<index - 1, types...> next;
            size_t b = std::hash<typename std::decay<nexttype>::type>()(std::get<index>(t));
            return next(hash_combiner(a, b), t);
        }
    };
    template<class...types>
    struct hash_impl<0, types...> {
        size_t operator()(size_t a, const std::tuple<types...>& t) const {
            typedef typename std::tuple_element<0, std::tuple<types...>>::type nexttype;
            size_t b = std::hash<typename std::decay<nexttype>::type>()(std::get<0>(t));
            return hash_combiner(a, b);
        }
    };
}

// specialization for std::hash<std::tuple<...>>
namespace std {
    template<class...types>
    struct hash<std::tuple<types...>> {
        size_t operator()(const std::tuple<types...>& t) const {
            const size_t begin = std::tuple_size<std::tuple<types...>>::value - 1;
            return hash_impl<begin, types...>()(0, t);
        }
    };
    
    template<class First, class Second>
    struct hash<std::pair<First, Second>> {
        size_t operator()(const std::pair<First, Second>& t) const {
            return hash_impl<1, First, Second>()(0, t);
        }
    };
    
    template<class T>
    struct hash<std::vector<T>> {
        size_t operator()(const std::vector<T>& v) const {
            size_t input = 0x9e3779b9;
            return std::accumulate<>(v.begin(), v.end(), input, [](const size_t& a, const T& b) {
                return hash_combiner(a, std::hash<T>{}(b));
            });
        }
    };
    
    template<class T, std::size_t N>
    struct hash<std::array<T, N>> {
        size_t operator()(const std::array<T, N>& v) const {
            size_t input = 0x9e3779b9;
            return std::accumulate<>(v.begin(), v.end(), input, [](const size_t& a, const T& b) {
                return hash_combiner(a, std::hash<T>{}(b));
            });
        }
    };
}

static constexpr float MATH_HALFPI    = 1.570796f;
static constexpr float MATH_PI        = 3.141592f;
static constexpr float MATH_2PI       = 6.283184f;

template<typename T>
constexpr T DegreesToRadians(const T angle) { return static_cast<T>(angle * MATH_PI / 180); }

template<typename T>
constexpr T RadiansToDegrees(const T angle) { return static_cast<T>(angle * 180 / MATH_PI); }

class Vec3
{
public:
    Vec3() = default;
    
    Vec3(float x, float y, float z)
    {
        new (&mStorage) simd::float3;
        
        simd::float3& data = *reinterpret_cast<simd::float3*>((void*)&mStorage);
        data.x = x;
        data.y = y;
        data.z = z;
    }
    
    bool operator==(const Vec3& other) const noexcept
    {
        simd::float3& thisData = *reinterpret_cast<simd::float3*>((void*)&mStorage);
        simd::float3& otherData = *reinterpret_cast<simd::float3*>((void*)&other.mStorage);
        
        return  thisData.x == otherData.x &&
                thisData.y == otherData.y &&
                thisData.z == otherData.z;
    }
    
    float x() const noexcept
    {
        simd::float3& data = *reinterpret_cast<simd::float3*>((void*)&mStorage);
        return data.x;
    }
    
    float y() const noexcept
    {
        simd::float3& data = *reinterpret_cast<simd::float3*>((void*)&mStorage);
        return data.y;
    }
    
    float z() const noexcept
    {
        simd::float3& data = *reinterpret_cast<simd::float3*>((void*)&mStorage);
        return data.z;
    }
    
    void SetX(float x)
    {
        simd::float3& data = *reinterpret_cast<simd::float3*>((void*)&mStorage);
        data.x = x;
    }
    
    void AddX(float x)
    {
        simd::float3& data = *reinterpret_cast<simd::float3*>((void*)&mStorage);
        data.x += x;
    }
    
private:
    std::aligned_storage<sizeof(simd::float3), 16>::type mStorage;
};

class Vec4
{
public:
    Vec4() = default;
    
    Vec4(float x, float y, float z, float w)
    {
        new (&mStorage) simd::float4;
        
        simd::float4& data = *reinterpret_cast<simd::float4*>((void*)&mStorage);
        data.x = x;
        data.y = y;
        data.z = z;
        data.w = w;
    }
    
    bool operator==(const Vec4& other) const noexcept
    {
        simd::float4& thisData = *reinterpret_cast<simd::float4*>((void*)&mStorage);
        simd::float4& otherData = *reinterpret_cast<simd::float4*>((void*)&other.mStorage);
        
        return  thisData.x == otherData.x &&
                thisData.y == otherData.y &&
                thisData.z == otherData.z &&
                thisData.w == otherData.w;
    }
    
    float x() const noexcept
    {
        simd::float4& data = *reinterpret_cast<simd::float4*>((void*)&mStorage);
        return data.x;
    }
    
    float y() const noexcept
    {
        simd::float4& data = *reinterpret_cast<simd::float4*>((void*)&mStorage);
        return data.y;
    }
    
    float z() const noexcept
    {
        simd::float4& data = *reinterpret_cast<simd::float4*>((void*)&mStorage);
        return data.z;
    }
    
    float w() const noexcept
    {
        simd::float4& data = *reinterpret_cast<simd::float4*>((void*)&mStorage);
        return data.w;
    }
    
private:
    std::aligned_storage<16, 16>::type mStorage;
};

namespace std
{
    template<>
    struct hash<Math::Float2>
    {
        size_t operator()(const Math::Float2& vec) const
        {
            const float x = vec.x;
            const float y = vec.y;
            
            auto t = std::tie(x, y);
            return std::hash<decltype(t)>()(t);
        }
    };
}

namespace std
{
    template<>
    struct hash<Vec3>
    {
        size_t operator()(const Vec3& vec) const
        {
            const float x = vec.x();
            const float y = vec.y();
            const float z = vec.z();
            
            auto t = std::tie(x, y, z);
            return std::hash<decltype(t)>()(t);
        }
    };
}

namespace std
{
    template<>
    struct hash<Vec4>
    {
        size_t operator()(const Vec4& vec) const
        {
            const float x = vec.x();
            const float y = vec.y();
            const float z = vec.z();
            const float w = vec.w();
            
            auto t = std::tie(x, y, z, w);
            return std::hash<decltype(t)>()(t);
        }
    };
}
