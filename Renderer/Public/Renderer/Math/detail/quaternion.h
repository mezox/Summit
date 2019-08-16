#pragma once

#include "defines.h"

#include <ostream>

namespace math::detail
{
    template<typename T>
    class quaternion
    {
    public:
        CONSTEXPR_QUALIFIER quaternion() noexcept;
        CONSTEXPR_QUALIFIER quaternion(T x, T y, T z, T w) noexcept;
        CONSTEXPR_QUALIFIER quaternion(const vector3<T>& axis, T radians) noexcept;
        
        quaternion& operator+=(const quaternion& other) noexcept;
        
        template<typename U>
        CONSTEXPR_QUALIFIER U length() const;
        
        template<typename U>
        CONSTEXPR_QUALIFIER U length_squared() const;
        
        CONSTEXPR_QUALIFIER T angle() const;
        //CONSTEXPR_QUALIFIER vector3<T> axis() const;
        
        CONSTEXPR_QUALIFIER void normalize();
        
        friend std::ostream& operator<<(std::ostream& s, const quaternion& q)
        {
            s << "q(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")\n";
            s << "q'(" << q.data.x << ", " << q.data.y << ", " << q.data.z << ", " << q.data.w << ")\n";
            
            return s;
        }
        
    public:
        union
        {
            struct
            {
                T x{}, y{}, z{}, w{};
            };
            
            typename detail::storage_t<4, T> data;
        };
    };
            
    template<typename T>
    quaternion<T> operator*(const quaternion<T>& q0, const quaternion<T>& q1)
    {
        quaternion<T> q;
        q.x = q0.data.w * q1.data.x + q0.data.x * q1.data.w + q0.data.y * q1.data.z - q0.data.z * q1.data.y;
        q.y = q0.data.w * q1.data.y - q0.data.x * q1.data.z + q0.data.y * q1.data.w + q0.data.z * q1.data.x;
        q.z = q0.data.w * q1.data.z + q0.data.x * q1.data.y - q0.data.y * q1.data.x + q0.data.z * q1.data.w;
        q.w = q0.data.w * q1.data.w - q0.data.x * q1.data.x - q0.data.y * q1.data.y - q0.data.z * q1.data.z;
        return q;
    }
            
    template<typename T>
    struct quaternion_init
    {
        static void execute(T x, T y, T z, T w, quaternion<T>& q)
        {
            static_assert(is_float<T>::value, "Required float type");
            q.x = x;
            q.y = y;
            q.z = z;
            q.w = w;
        }
    };
            
    template<typename T>
    inline CONSTEXPR_QUALIFIER quaternion<T>::quaternion() noexcept
    {
        quaternion_init<T>::execute(0, 0, 0, 1, *this);
    }
        
    template<typename T>
    inline CONSTEXPR_QUALIFIER quaternion<T>::quaternion(T x, T y, T z, T w) noexcept
    {
        quaternion_init<T>::execute(x, y, z, w, *this);
    }
      
    template<typename T>
    inline CONSTEXPR_QUALIFIER quaternion<T>::quaternion(const vector3<T>& axis, T radians) noexcept
    {
        CONSTEXPR_QUALIFIER float t = radians * 0.5;
        CONSTEXPR_QUALIFIER float sint = sinf(t);
        CONSTEXPR_QUALIFIER float cost = cosf(t);
        
        quaternion_init<T>::execute(axis.x * sint, axis.y * sint, axis.z * sint, cost, *this);
    }

    template<typename T>
    struct compute_quaternion_add
    {
        static quaternion<T> execute(const quaternion<T>& a, const quaternion<T>& b)
        {
            static_assert(is_float<T>::value, "Required float type");
            
            return quaternion<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
        }
    };
            
    template<typename T, typename U>
    struct compute_quaternion_length
    {
        static U execute(const quaternion<T>& q)
        {
            static_assert(is_float<T>::value, "Required float type");
            return static_cast<U>(std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w));
        }
    };
            
    template<typename T, typename U>
    struct compute_quaternion_length_squared
    {
        static U execute(const quaternion<T>& q)
        {
            static_assert(is_float<T>::value, "Required float type");
            return static_cast<U>(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
        }
    };
            
    template<typename T>
    struct compute_quaternion_normalize
    {
        static void execute(const quaternion<T>& q)
        {
            static_assert(is_float<T>::value, "Required float type");
            return;
        }
    };
            
    template<typename T>
    inline quaternion<T>& quaternion<T>::operator+=(const quaternion<T>& other) noexcept
    {
        return (*this = compute_quaternion_add<T>::execute(*this, other));
    }
    
    template<typename T>
    template<typename U>
    inline CONSTEXPR_QUALIFIER U quaternion<T>::length() const
    {
        return compute_quaternion_length<T, U>::execute(*this);
    }
    
    template<typename T>
    template<typename U>
    inline CONSTEXPR_QUALIFIER U quaternion<T>::length_squared() const
    {
        return compute_quaternion_length_squared<T, U>::execute(*this);
    }
            
    template<typename T>
    inline CONSTEXPR_QUALIFIER T quaternion<T>::angle() const
    {
        return 2 * acosf(data.w);
    }
            
    template<typename T>
    inline CONSTEXPR_QUALIFIER void quaternion<T>::normalize()
    {
        return compute_quaternion_normalize<T>::execute(*this);
    }
    
//    template<typename T>
//    inline CONSTEXPR_QUALIFIER vector3<T> quaternion<T>::axis() const
//    {
//        return compute_quaternion_axis<T>::execute(*this);
//    }
}
            
#if defined(APPLE_SIMD)
#include "simd/quaternion_simd.h"
#endif
