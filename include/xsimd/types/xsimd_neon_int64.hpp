/***************************************************************************
* Copyright (c) 2016, Wolf Vollprecht, Johan Mabille and Sylvain Corlay    *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XSIMD_NEON_INT64_HPP
#define XSIMD_NEON_INT64_HPP

#include "xsimd_base.hpp"

namespace xsimd
{

    /*********************
     * batch<int64_t, 2> *
     *********************/

    template <>
    struct simd_batch_traits<batch<int64_t, 2>>
    {
        using value_type = int64_t;
        static constexpr std::size_t size = 2;
        using batch_bool_type = batch_bool<int64_t, 2>;
        static constexpr std::size_t align = XSIMD_DEFAULT_ALIGNMENT;
    };

    template <>
    class batch<int64_t, 2> : public simd_batch<batch<int64_t, 2>>
    {
        using simd_type = int64x2_t;

    public:

        batch();
        explicit batch(int64_t d);
        batch(int64_t d0, int64_t d1);
        explicit batch(const int64_t* src);

        batch(const int64_t* src, aligned_mode);
        batch(const int64_t* src, unaligned_mode);

        batch(const simd_type& rhs);
        batch& operator=(const simd_type& rhs);

        operator simd_type() const;

        batch& load_aligned(const float* src);
        batch& load_unaligned(const float* src);

        batch& load_aligned(const double* src);
        batch& load_unaligned(const double* src);

        batch& load_aligned(const int32_t* src);
        batch& load_unaligned(const int32_t* src);

        batch& load_aligned(const int64_t* src);
        batch& load_unaligned(const int64_t* src);

        batch& load_aligned(const char* src);
        batch& load_unaligned(const char* src);

        batch& load_aligned(const unsigned char* src);
        batch& load_unaligned(const unsigned char* src);

        void store_aligned(float* dst) const;
        void store_unaligned(float* dst) const;

        void store_aligned(double* dst) const;
        void store_unaligned(double* dst) const;

        void store_aligned(int32_t* dst) const;
        void store_unaligned(int32_t* dst) const;

        void store_aligned(int64_t* dst) const;
        void store_unaligned(int64_t* dst) const;

        void store_aligned(char* dst) const;
        void store_unaligned(char* dst) const;

        void store_aligned(unsigned char* dst) const;
        void store_unaligned(unsigned char* dst) const;

        int64_t operator[](std::size_t index) const;

    private:

        simd_type m_value;
    };

    batch<int64_t, 2> operator<<(const batch<int64_t, 2>& lhs, int32_t rhs);
    batch<int64_t, 2> operator>>(const batch<int64_t, 2>& lhs, int32_t rhs);

    /***********************************
    * batch<int64_t, 2> implementation *
    ************************************/

    inline batch<int64_t, 2>::batch()
    {
    }

    inline batch<int64_t, 2>::batch(int64_t d)
        : m_value(vdupq_n_s64(d))
    {
    }

    inline batch<int64_t, 2>::batch(int64_t d1, int64_t d2)
        : m_value{d1, d2}
    {
    }

    inline batch<int64_t, 2>::batch(const int64_t* d)
        : m_value(vld1q_s64(d))
    {
    }

    inline batch<int64_t, 2>::batch(const int64_t* d, aligned_mode)
        : batch(d)
    {
    }

    inline batch<int64_t, 2>::batch(const int64_t* d, unaligned_mode)
        : batch(d)
    {
    }

    inline batch<int64_t, 2>::batch(const simd_type& rhs)
        : m_value(rhs)
    {
    }

    inline batch<int64_t, 2>& batch<int64_t, 2>::operator=(const simd_type& rhs)
    {
        m_value = rhs;
        return *this;
    }

    inline batch<int64_t, 2>& batch<int64_t, 2>::load_aligned(const float* d)
    {
    #if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION
        m_value = vcvtq_s64_f64(vcvt_f64_f32(vld1_f32(d)));
    #else
        m_value = int64x2_t{static_cast<int64_t>(d[0]),
                            static_cast<int64_t>(d[1])};
    #endif
        return *this;
    }

    inline batch<int64_t, 2>& batch<int64_t, 2>::load_unaligned(const float* d)
    {
        return load_unaligned(d);
    }

    inline batch<int64_t, 2>& batch<int64_t, 2>::load_aligned(const double* d)
    {
    #if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION
        m_value = vcvtq_s64_f64(vld1q_f64(d));
    #else
        m_value = int64x2_t{static_cast<int64_t>(d[0]),
                            static_cast<int64_t>(d[1])};
    #endif
        return *this;
    }

    inline batch<int64_t, 2>& batch<int64_t, 2>::load_unaligned(const double* d)
    {
        return load_aligned(d);
    }

    inline batch<int64_t, 2>& batch<int64_t, 2>::load_aligned(const int32_t* d)
    {
        m_value = int64x2_t{d[0], d[1]};
        return *this;
    }

    inline batch<int64_t, 2>& batch<int64_t, 2>::load_unaligned(const int32_t* d)
    {
        return load_aligned(d);
    }

    inline batch<int64_t, 2>& batch<int64_t, 2>::load_aligned(const int64_t* d)
    {
        m_value = vld1q_s64(d);
        return *this;
    }

    inline batch<int64_t, 2>& batch<int64_t, 2>::load_unaligned(const int64_t* d)
    {
        return load_aligned(d);
    }

    inline batch<int64_t, 2>& batch<int64_t, 2>::load_aligned(const char* src)
    {
        int8x8_t tmp = vld1_s8((const int8_t*)src);
        int16x8_t tmp2 = vmovl_s8(tmp);
        int16x4_t tmp3 = vget_low_s16(tmp2);
        int32x4_t tmp4 = vmovl_s16(tmp3);
        int32x2_t tmp5 = vget_low_s32(tmp4);
        m_value = vmovl_s32(tmp5);
        return *this;
    }

    inline batch<int64_t, 2>& batch<int64_t, 2>::load_unaligned(const char* src)
    {
        return load_aligned(src);
    }

    inline batch<int64_t, 2>& batch<int64_t, 2>::load_aligned(const unsigned char* src)
    {
        uint8x8_t tmp = vld1_u8((const uint8_t*)src);
        uint16x8_t tmp2 = vmovl_u8(tmp);
        uint16x4_t tmp3 = vget_low_u16(tmp2);
        uint32x4_t tmp4 = vmovl_u16(tmp3);
        uint32x2_t tmp5 = vget_low_u32(tmp4);
        m_value = vmovl_u32(tmp5);
        return *this;
    }

    inline batch<int64_t, 2>& batch<int64_t, 2>::load_unaligned(const unsigned char* src)
    {
        return load_aligned(src);
    }

    inline void batch<int64_t, 2>::store_aligned(float* dst) const
    {
    #if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION
        vst1_f32(dst, vcvt_f32_f64(vcvtq_f64_s64(m_value)));
    #else
        dst[0] = static_cast<float>(m_value[0]);
        dst[1] = static_cast<float>(m_value[1]);
    #endif
    }

    inline void batch<int64_t, 2>::store_unaligned(float* dst) const
    {
        store_aligned(dst);
    }

    inline void batch<int64_t, 2>::store_aligned(double* dst) const
    {
    #if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION
        vst1q_f64(dst, vcvtq_f64_s64(m_value));
    #else
        dst[0] = static_cast<double>(m_value[0]);
        dst[1] = static_cast<double>(m_value[1]);
    #endif
    }

    inline void batch<int64_t, 2>::store_unaligned(double* dst) const
    {
        store_aligned(dst);
    }

    inline void batch<int64_t, 2>::store_aligned(int32_t* dst) const
    {
        dst[0] = static_cast<int32_t>(m_value[0]);
        dst[1] = static_cast<int32_t>(m_value[1]);
    }

    inline void batch<int64_t, 2>::store_unaligned(int32_t* dst) const
    {
        store_aligned(dst);
    }

    inline void batch<int64_t, 2>::store_aligned(int64_t* dst) const
    {
        vst1q_s64(dst, m_value);
    }

    inline void batch<int64_t, 2>::store_unaligned(int64_t* dst) const
    {
        store_aligned(dst);
    }

    inline void batch<int64_t, 2>::store_aligned(char* dst) const
    {
        int32x2_t tmp2 = vmovn_s64(m_value);
        int32x4_t tmp3 = vcombine_s32(tmp2, vdup_n_s32(0));
        int16x4_t tmp4 = vmovn_s32(tmp3);
        int16x8_t tmp5 = vcombine_s16(tmp4, vdup_n_s16(0));
        int8x8_t tmp6 = vmovn_s16(tmp5);
        vst1_s8((int8_t*)dst, tmp6);
    }

    inline void batch<int64_t, 2>::store_unaligned(char* dst) const
    {
        store_aligned(dst);
    }

    inline void batch<int64_t, 2>::store_aligned(unsigned char* dst) const
    {
        uint32x2_t tmp2 = vmovn_u64(m_value);
        uint32x4_t tmp3 = vcombine_u32(tmp2, vdup_n_u32(0));
        uint16x4_t tmp4 = vmovn_u32(tmp3);
        uint16x8_t tmp5 = vcombine_u16(tmp4, vdup_n_u16(0));
        uint8x8_t tmp6 = vmovn_u16(tmp5);
        vst1_u8((uint8_t*)dst, tmp6);
    }

    inline void batch<int64_t, 2>::store_unaligned(unsigned char* dst) const
    {
        store_aligned(dst);
    }

    inline batch<int64_t, 2>::operator int64x2_t() const
    {
        return m_value;
    }

    inline int64_t batch<int64_t, 2>::operator[](std::size_t index) const
    {
        return m_value[index];
    }

    namespace detail
    {
        template <>
        struct batch_kernel<int64_t, 2>
        {
            using batch_type = batch<int64_t, 2>;
            using value_type = int64_t;
            using batch_bool_type = batch_bool<int64_t, 2>;

            static batch_type neg(const batch_type& rhs)
            {
#if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION
                return vnegq_s64(rhs);
#else
                return batch<int64_t, 2>(-rhs[0], -rhs[1]);
#endif
            }

            static batch_type add(const batch_type& lhs, const batch_type& rhs)
            {
                return vaddq_s64(lhs, rhs);
            }

            static batch_type sub(const batch_type& lhs, const batch_type& rhs)
            {
                return vsubq_s64(lhs, rhs);
            }

            static batch_type mul(const batch_type& lhs, const batch_type& rhs)
            {
                return { lhs[0] * rhs[0], lhs[1] * rhs[1] };
            }

            static batch_type div(const batch_type& lhs, const batch_type& rhs)
            {
#if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION && defined(XSIMD_FAST_INTEGER_DIVISION)
                return vcvtq_s64_f64(vcvtq_f64_s64(lhs) / vcvtq_f64_s64(rhs));
#else
                return{ lhs[0] / rhs[0], lhs[1] / rhs[1] };
#endif
            }

            static batch_type mod(const batch_type& lhs, const batch_type& rhs)
            {
                return{ lhs[0] % rhs[0], lhs[1] % rhs[1] };
            }

            static batch_bool_type eq(const batch_type& lhs, const batch_type& rhs)
            {
#if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION
                return vceqq_s64(lhs, rhs);
#else
                return batch_bool<int64_t, 2>(lhs[0] == rhs[0], lhs[1] == rhs[1]);
#endif
            }

            static batch_bool_type neq(const batch_type& lhs, const batch_type& rhs)
            {
                return !(lhs == rhs);
            }

            static batch_bool_type lt(const batch_type& lhs, const batch_type& rhs)
            {
#if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION
                return vcltq_s64(lhs, rhs);
#else
                return batch_bool<int64_t, 2>(lhs[0] < rhs[0], lhs[1] < rhs[1]);
#endif
            }

            static batch_bool_type lte(const batch_type& lhs, const batch_type& rhs)
            {
#if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION
                return vcleq_s64(lhs, rhs);
#else
                return batch_bool<int64_t, 2>(lhs[0] <= rhs[0], lhs[1] <= rhs[1]);
#endif
            }

            static batch_type bitwise_and(const batch_type& lhs, const batch_type& rhs)
            {
                return vandq_s64(lhs, rhs);
            }

            static batch_type bitwise_or(const batch_type& lhs, const batch_type& rhs)
            {
                return vorrq_s64(lhs, rhs);
            }

            static batch_type bitwise_xor(const batch_type& lhs, const batch_type& rhs)
            {
                return veorq_s64(lhs, rhs);
            }

            static batch_type bitwise_not(const batch_type& rhs)
            {
                return vreinterpretq_s64_s32(vmvnq_s32(vreinterpretq_s32_s64(rhs)));
            }

            static batch_type bitwise_andnot(const batch_type& lhs, const batch_type& rhs)
            {
                return vbicq_u64(lhs, rhs);
            }

            static batch_type min(const batch_type& lhs, const batch_type& rhs)
            {
                return { lhs[0] < rhs[0] ? lhs[0] : rhs[0],
                         lhs[1] < rhs[1] ? lhs[1] : rhs[1] };
            }

            static batch_type max(const batch_type& lhs, const batch_type& rhs)
            {
                return { lhs[0] > rhs[0] ? lhs[0] : rhs[0],
                         lhs[1] > rhs[1] ? lhs[1] : rhs[1] };
            }

            static batch_type abs(const batch_type& rhs)
            {
#if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION
                return vabsq_s64(rhs);
#else
                return batch<int64_t, 2>(std::abs(rhs[0]), std::abs(rhs[1]));
#endif
            }

            static batch_type fma(const batch_type& x, const batch_type& y, const batch_type& z)
            {
                return x * y + z;
            }

            static batch_type fms(const batch_type& x, const batch_type& y, const batch_type& z)
            {
                return x * y - z;
            }

            static batch_type fnma(const batch_type& x, const batch_type& y, const batch_type& z)
            {
                return -x * y + z;
            }

            static batch_type fnms(const batch_type& x, const batch_type& y, const batch_type& z)
            {
                return -x * y - z;
            }

            static value_type hadd(const batch_type& rhs)
            {
#if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION
                return vaddvq_s64(rhs);
#else
                return rhs[0] + rhs[1];
#endif
            }

            static batch_type select(const batch_bool_type& cond, const batch_type& a, const batch_type& b)
            {
                return vbslq_s64(cond, a, b);
            }
        };
    }



    /*inline batch<int64_t, 2> haddp(const batch<int64_t, 2>* row)
    {
    #if XSIMD_ARM_INSTR_SET >= XSIMD_ARM8_64_NEON_VERSION
        return vpaddq_s64(row[0], row[1]);
    #else
        return batch<int64_t, 2>(row[0][0] + row[0][1], row[1][0] + row[1][1]);
    #endif
    }*/

    namespace detail
    {
        inline batch<int64_t, 2> shift_left(const batch<int64_t, 2>& lhs, const int n)
        {
            switch(n)
            {
                case 0: return lhs;
                REPEAT_32(vshlq_n_s64, 0);
                REPEAT_32(vshlq_n_s64, 31);
                case 63: return vshlq_n_s64(lhs, 63); break;
                default: break;
            }
            return batch<int64_t, 2>(int64_t(0));
        }

        inline batch<int64_t, 2> shift_right(const batch<int64_t, 2>& lhs, const int n)
        {
            switch(n)
            {
                case 0: return lhs;
                REPEAT_32(vshrq_n_s64, 0);
                REPEAT_32(vshrq_n_s64, 31);
                case 63: return vshrq_n_s64(lhs, 63); break;
                default: break;
            }
            return batch<int64_t, 2>(int64_t(0));
        }
    }

    inline batch<int64_t, 2> operator<<(const batch<int64_t, 2>& lhs, int32_t rhs)
    {
        return detail::shift_left(lhs, rhs);
    }

    inline batch<int64_t, 2> operator>>(const batch<int64_t, 2>& lhs, int32_t rhs)
    {
        return detail::shift_right(lhs, rhs);
    }

    inline batch<int64_t, 2> operator<<(const batch<int64_t, 2>& lhs, const batch<int64_t, 2>& rhs)
    {
        return vshlq_s64(lhs, rhs);
    }
}

#endif
