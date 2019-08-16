#pragma once

#include "defines.h"
#include "vector.h"

#include <ostream>
#include <iomanip>

#define APPLE_SIMD

namespace math::detail
{
    template<typename T>
    class quaternion;

    template<typename T>
    class matrix4x4
    {
    public:
        CONSTEXPR_QUALIFIER matrix4x4() noexcept;
        CONSTEXPR_QUALIFIER explicit matrix4x4(const quaternion<T>& q) noexcept;
        
        CONSTEXPR_QUALIFIER void make_identity();
        CONSTEXPR_QUALIFIER void make_inverse();
        CONSTEXPR_QUALIFIER matrix4x4 inverse() const;
        
        friend std::ostream& operator<<(std::ostream& s, const matrix4x4& m)
        {
            for(uint8_t i{ 0 } ; i < 4; ++i)
            {
                for(uint8_t j{ 0 } ; j < 4; ++j)
                {
                    s << std::right << std::fixed << std::setw(7) << std::setprecision(3) << m.data.columns[j][i];
                    }
                    
                    s << "\n";
                }
                    
                return s;
        }
        
    public:        
        union
        {
            vector4<T> columns[4];
            typename detail::storage_t<16, T> data;
        };
    };
    
    template<typename T>
    struct matrix4x4_init
    {
        static void execute(matrix4x4<T>& m)
        {
            static_assert(is_float<T>::value, "Required float type");
        }
    };

    template<typename T>
    struct matrix4x4_init_quaternion
    {
        static void execute(matrix4x4<T>& m, const quaternion<T>& q)
        {
            static_assert(true, "Not implemented for non SIMD");
        }
    };

    template<typename T>
    struct matrix4x4_compute_inverse : std::false_type
    {
        static void execute(const matrix4x4<T>& m)
        {
            static_assert(true, "Not implemented for non SIMD");
        }
    };

    template<typename T>
    struct matrix4x4_make_translation : std::false_type
    {
        static void execute(matrix4x4<T>& m, const vector3<T>& v)
        {
            static_assert(true, "Not implemented for non SIMD");
        }
    };

    template<typename T>
    struct matrix4x4_multiply : std::false_type
    {};
    
    template<typename T>
    inline CONSTEXPR_QUALIFIER matrix4x4<T>::matrix4x4() noexcept
    {
        matrix4x4_init<T>::execute(*this);
    }

    template<typename T>
    inline CONSTEXPR_QUALIFIER matrix4x4<T>::matrix4x4(const quaternion<T>& q) noexcept
    {
        matrix4x4_init_quaternion<T>::execute(*this, q);
    }

    template<typename T>
    inline CONSTEXPR_QUALIFIER void matrix4x4<T>::make_identity()
    {
        matrix4x4_init<T>::execute(*this);
    }

    template<typename T>
    inline CONSTEXPR_QUALIFIER void matrix4x4<T>::make_inverse()
    {
        *this = matrix4x4_compute_inverse<T>::execute(*this);
    }

    template<typename T>
    inline CONSTEXPR_QUALIFIER matrix4x4<T> matrix4x4<T>::inverse() const
    {
        return matrix4x4_compute_inverse<T>::execute(*this);
    }

    template<typename T>
    inline CONSTEXPR_QUALIFIER matrix4x4<T> operator*(const matrix4x4<T>& a, const matrix4x4<T>& b)
    {
        return matrix4x4_multiply<T>::execute(a, b);
    }
}

#ifdef APPLE_SIMD
#include "simd/matrix_simd.h"
#endif
