#pragma once

#include "defines.h"
#include <ostream>

#define APPLE_SIMD

namespace math::detail
{
    template<typename T>
    class vector2
    {
    public:
        CONSTEXPR_QUALIFIER vector2() noexcept;
        CONSTEXPR_QUALIFIER vector2(T x, T y) noexcept;
        
        CONSTEXPR_QUALIFIER bool operator==(const vector2& other) const;

            
    public:
        union
        {
            struct { T x, y; };
            struct { T u, v; };
            typename detail::storage_t<2, T> data;
        };
    };
            
            
    template<typename T>
    struct vector2_init
    {
        static void execute(T x, T y, vector2<T>& v)
        {
            static_assert(is_float<T>::value, "Required float type");
            v.x = x;
            v.y = y;
        }
    };
    
    template<typename T>
    inline CONSTEXPR_QUALIFIER vector2<T>::vector2() noexcept
    {
        vector2_init<T>::execute(0, 0, *this);
    }
    
    template<typename T>
    inline CONSTEXPR_QUALIFIER vector2<T>::vector2(T x, T y) noexcept
    {
        vector2_init<T>::execute(x, y, *this);
    }
    
    template<typename T>
    inline CONSTEXPR_QUALIFIER bool vector2<T>::operator==(const vector2& other) const
    {
        return data.x == other.data.x && data.y == other.data.y;
    }
    
    template<typename T>
    struct compute_vector2_add
    {
        static vector2<T> execute(const vector2<T>& a, const vector2<T>& b)
        {
            static_assert(is_float<T>::value, "Required float type");
            
            return vector2<T>(a.x + b.x, a.y + b.y);
        }
    };
    
    
    
    //Vector3
    template<typename T>
    class vector3
    {
    public:
        CONSTEXPR_QUALIFIER vector3() noexcept;
        CONSTEXPR_QUALIFIER vector3(T x, T y, T z) noexcept;
        
        CONSTEXPR_QUALIFIER bool operator==(const vector3& other) const;
        
        
    public:
        union
        {
            struct { T x, y, z; };
            typename detail::storage_t<3, T> data;
        };
    };
    
    
    template<typename T>
    struct vector3_init
    {
        static void execute(T x, T y, T z, vector3<T>& v)
        {
            static_assert(is_float<T>::value, "Required float type");
            v.x = x;
            v.y = y;
            v.z = z;
        }
    };
    
    template<typename T>
    inline CONSTEXPR_QUALIFIER vector3<T>::vector3() noexcept
    {
        vector3_init<T>::execute(0, 0, 0, *this);
    }
    
    template<typename T>
    inline CONSTEXPR_QUALIFIER vector3<T>::vector3(T x, T y, T z) noexcept
    {
        vector3_init<T>::execute(x, y, z, *this);
    }
    
    template<typename T>
    inline CONSTEXPR_QUALIFIER bool vector3<T>::operator==(const vector3& other) const
    {
        return data.x == other.data.x && data.y == other.data.y && data.z == other.data.z;
    }
    
    //Vector4
    template<typename T>
    class vector4
    {
    public:
        CONSTEXPR_QUALIFIER vector4() noexcept;
        CONSTEXPR_QUALIFIER vector4(T x, T y, T z, T w) noexcept;
        
    public:
        union
        {
            struct { T x, y, z, w; };
            typename detail::storage_t<4, T> data;
        };
    };
    
    
    template<typename T>
    struct vector4_init
    {
        static void execute(T x, T y, T z, T w, vector4<T>& v)
        {
            static_assert(is_float<T>::value, "Required float type");
            v.x = x;
            v.y = y;
            v.z = z;
            v.w = w;
        }
    };
    
    template<typename T>
    inline CONSTEXPR_QUALIFIER vector4<T>::vector4() noexcept
    {
        vector4_init<T>::execute(0, 0, 0, 0, *this);
    }
    
    template<typename T>
    inline CONSTEXPR_QUALIFIER vector4<T>::vector4(T x, T y, T z, T w) noexcept
    {
        vector4_init<T>::execute(x, y, z, w, *this);
    }
}
            
#ifdef APPLE_SIMD
#include "simd/vector_simd.h"
#endif
