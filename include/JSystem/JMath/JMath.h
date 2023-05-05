#ifndef JMATH_H
#define JMATH_H

#include <dolphin/mtx.h>
#include <dolphin/math.h>
#include <std/pair.h>
#include "types.h"

namespace JMath
{
    template <int length, typename T>
    struct TSinCosTable
    {
        /**
         * elements are pairs of {sine, cosine}
         */
        std::pair<T, T> mTable[length];
    };

    template <int length, typename T>
    struct TAtanTable
    {
        T atan2_(T, T) const;
        T atan_(T) const;
        T mTable[length];
    };

    template <int length, typename T>
    struct TAsinAcosTable
    {
        T acos2_(T, T) const;
        T acos_(T) const;
        T mTable[length];
    };

    template <>
    struct TSinCosTable<2048, f32>
    {
        TSinCosTable()
        {
            f64 tau = LONG_TAU;
            for (int i = 0; i < 2048; i++)
            {
                mTable[i].first = sin((i * tau) / 2048.0); // there must be some sort of inline/macro for this
                mTable[i].second = cos((i * tau) / 2048.0);
            }
        }
        /**
         * elements are pairs of {sine, cosine}
         */
        f32 sinShort(s16 v) const { return mTable[static_cast<u16>(v) >> 5].first; }
        f32 cosShort(s16 v) const { return mTable[static_cast<u16>(v) >> 5].second; }
        std::pair<f32, f32> mTable[2048];
    };

    template <>
    struct TAtanTable<1024, f32>
    {        
        TAtanTable()
        {
            f64 constval = 9.765625E-4;
            for (int i = 0; i < (u32)1024; i++)
            {
                mTable[i] = (f32)atan(i * constval);
            }
            mTable[0] = 0.0f;
            _0x400 = 0.25f * PI;
        }

        f32 atan_(f32) const;
        f32 atan2_(f32, f32) const;
        f32 atan2Radian(f32 y, f32 x) const { return atan2_(y, x); }
        f32 alignmentHack(f32 x) const;

        f32 calc(f32 y, f32 x) const
        {
            if (x >= 0.0f)
            {
                if (x >= y)
                {
                    return (0.0f == x ? 0.0f : mTable[(int)((y * 1024.0f) / x + 0.5f)]);
                }
                else
                {
                    return HALF_PI - (y == 0.0f ? 0.0f : mTable[(int)((x * 1024.0f) / y + 0.5f)]);
                }
            }
            else
            {
                x = -x;
                if (x < y)
                {
                    return (y == 0.0f ? 0.0f : mTable[(int)((x * 1024.0f) / y + 0.5f)]) + HALF_PI;
                }
                else
                {
                    return PI - (x == 0.0f ? 0.0f : mTable[(int)((y * 1024.0f) / x + 0.5f)]);
                }
            }
        }

        f32 calcInverse(f32 y, f32 x) const
        {
            y = -y;
            if (x < 0.0f)
            {
                x = -x;
                if (x >= y)
                {
                    return (x == 0.0f ? 0.0f : mTable[(int)((y * 1024.0f) / x + 0.5f)]) + -PI;
                }
                else
                {
                    return -HALF_PI - (y == 0.0f ? 0.0f : mTable[(int)((x * 1024.0f) / y + 0.5f)]);
                }
            }
            else
            {
                if (x < y)
                {
                    return (y == 0.0f ? 0.0f : mTable[(int)((x * 1024.0f) / y + 0.5f)]) + -HALF_PI;
                }
                else
                {
                    return -(x == 0.0f ? 0.0f : mTable[(int)((y * 1024.0f) / x + 0.5f)]);
                }
            }
        }

        f32 mTable[1024];
        f32 _0x400;
    };

    template <>
    struct TAsinAcosTable<1024, f32>
    {
        TAsinAcosTable()
        {
            f64 constval = 9.765625E-4;
            for (int i = 0; i < 1024; i++)
            {
                mTable[i] = (f32)asin(i * constval);
            }
            mTable[0] = 0.0f;
            _0x400 = 0.25f * PI;
        }
        f32 acos2_(f32, f32) const;
        f32 acos_(f32) const;
        f32 mTable[1024];
        f32 _0x400;
    };

    extern const TAsinAcosTable<1024, f32> asinAcosTable_ __attribute__((aligned(32)));
    extern const TSinCosTable<2048, f32> sincosTable_ __attribute__((aligned(32)));
    extern const TAtanTable<1024, f32> atanTable_ __attribute__((aligned(32)));

    class TRandom_fast_
    {
    public:
        TRandom_fast_(u32 num);
        u32 get()
        {
            seed = (seed * 0x19660d) + 0x3c6ef35f;
            return seed;
        }

        // from TP decomp
        f32 get_ufloat_1()
        {
            // !@bug UB: in C++ it's not legal to read from an union member other
            // than the last one that was written to.
            union
            {
                f32 f;
                u32 s;
            } out;
            out.s = (this->get() >> 9) | 0x3f800000;
            return out.f - 1;
        }

        void setSeed(u32 Seed)
        {
            seed = Seed;
        }

    protected:
        u32 seed;
    };
    template <typename T>
    class TRandom_ : public TRandom_fast_
    {
    public:
        TRandom_(u32 num) : TRandom_fast_(num){};
    };    

    inline const TSinCosTable<2048, f32> *getSinCosTable() { return &sincosTable_; }
}

inline f32 JMAAbs(f32 input) { return __fabs(input); }

inline f32 JMAAtan2Radian(f32 y, f32 x) { return JMath::atanTable_.atan2Radian(y, x); };

inline f32 JMASCosShort(s16 v) { return JMath::sincosTable_.cosShort(v); }
inline f32 JMASinShort(s16 v) { return JMath::sincosTable_.sinShort(v); }

inline f32 JMASCos(s16 v) { return JMASCosShort(v); }
inline f32 JMASSin(s16 v) { return JMASinShort(v); }

inline f32 JMAFastSqrt(register f32 x) {
    register f32 recip;

    if (x > 0.0f)
    {
        asm { frsqrte recip, x }
        return recip * x;
    }
    return x;
}

void JMAEulerToQuat(s16, s16, s16, Quaternion *);
void JMAQuatLerp(const Quaternion *, const Quaternion *, f32, Quaternion *);
void JMAVECScaleAdd(const Vec *, const Vec *, Vec *, f32);
void JMAVECLerp(const Vec *, const Vec *, Vec *, f32);
void JMAMTXApplyScale(const Mtx, Mtx, f32, f32, f32);

namespace JMathInlineVEC
{
    inline void PSVECAdd(register const Vec *vec1, register const Vec *vec2, register Vec *dst)
    {
        register f32 v1xy, v2xy, d1xy, v1z, v2z, d1z;
        __asm {
            // clang-format off
            psq_l     v1xy, 0(vec1), 0, 0
            psq_l     v2xy, 0(vec2), 0, 0
            ps_add    d1xy, v1xy, v2xy
            psq_st    d1xy, 0(dst), 0, 0
            
            psq_l     v1z,   8(vec1), 1, 0
            psq_l     v2z,   8(vec2), 1, 0
            ps_add    d1z, v1z, v2z
            psq_st    d1z,  8(dst), 1, 0
            // clang-format on
        }
    }
    inline void PSVECSubtract(register const Vec *vec1, register const Vec *vec2, register Vec *dst)
    {
        register f32 v1xy, v2xy, dxy, v1z, v2z, dz;
        __asm {
            // clang-format off
            psq_l     v1xy, 0(vec1), 0, 0
            psq_l     v2xy, 0(vec2), 0, 0
            ps_sub    dxy, v1xy, v2xy
            psq_st    dxy, 0(dst), 0, 0
            
            psq_l     v1z,   8(vec1), 1, 0
            psq_l     v2z,   8(vec2), 1, 0
            ps_sub    dz, v1z, v2z
            psq_st    dz,  8(dst), 1, 0
            // clang-format on
        }
    }
    inline void PSVECScale(register const Vec *src, register Vec *dst, register f32 scalar)
    {
        register f32 vxy, vz, rxy, rz;
        __asm {
            // clang-format off
            psq_l       vxy, 0(src), 0, 0
            psq_l       vz,  8(src), 1, 0
            ps_muls0    rxy, vxy, scalar
            ps_muls0    rz,  vz,  scalar
            psq_st      rxy, 0(dst), 0, 0            
            psq_st      rz,  8(dst), 1, 0
            // clang-format on
        }
    }
    inline f32 PSVECDotProduct(register const Vec *vec1, register const Vec *vec2)
    {
        register f32 dp, v1yz, v2xy, v2yz, v1xy;
        __asm {
            // clang-format off      
            psq_l    v1yz, 4(vec1), 0, 0
            psq_l    v2xy, 4(vec2), 0, 0 // typo? it's needed to match it for functions where this got inlined
            ps_mul   v1yz, v1yz, v2xy
            psq_l    v1xy, 0(vec1), 0, 0
            psq_l    v2xy, 0(vec2), 0, 0
            ps_madd  v2xy, v1xy, v2xy, v1yz
            ps_sum0  dp, v2xy, v1yz, v1yz
            // clang-format on
        }
        return dp;
    }
    inline f32 PSVECSquareMag(register const Vec *src)
    {
        register f32 xy, z, ret;
        __asm {
            psq_l xy, 0(src), 0, 0
            lfs z, 8(src)
            ps_mul xy, xy, xy
            ps_madd ret, z, z, xy
            ps_sum0 ret, ret, xy, xy
        }
        return ret;
    }
}

#endif // !JMATH_H
