#ifndef JMATH_H
#define JMATH_H

#include "types.h"
#include "dolphin/mtx.h"
#include "dolphin/math.h"
#include "std/pair.h"

// might belong to a different header
struct Quaternion {
	f32 x;
	f32 y;
	f32 z;
	f32 w;
};

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
    class TRandom_ : TRandom_fast_
    {
    public:
        TRandom_(u32 num) : TRandom_fast_(num){};
    };    

    inline const TSinCosTable<2048, f32> *getSinCosTable() { return &sincosTable_; }
}

inline f32 JMAAtan2Radian(f32 y, f32 x) { return JMath::atanTable_.atan2Radian(y, x); };

inline f32 JMASCosShort(s16 v) { return JMath::sincosTable_.cosShort(v); }
inline f32 JMASinShort(s16 v) { return JMath::sincosTable_.sinShort(v); }

inline f32 JMASCos(s16 v) { return JMASCosShort(v); }
inline f32 JMASSin(s16 v) { return JMASinShort(v); }

void JMAEulerToQuat(s16, s16, s16, Quaternion *);
void JMAQuatLerp(const Quaternion *, const Quaternion *, f32, Quaternion *);
void JMAVECScaleAdd(const Vec *, const Vec *, Vec *, f32);
void JMAVECLerp(const Vec *, const Vec *, Vec *, f32);
void JMAMTXApplyScale(const Mtx, Mtx, f32, f32, f32);

struct Vec // Typecast from TVec3<f32>
{
    f32 x;
    f32 y;
    f32 z;
};

namespace JMathInlineVEC
{
    inline void PSVECSubtract(const Vec *vec1, const Vec *vec2, Vec *dst); /*{
     dst->x = vec1->x - vec2->x;
     dst->y = vec1->y - vec2->y;
     dst->z = vec1->z - vec2->z;
 }*/

}

// fabricated
namespace JMathInlineQuat {
    inline f64 PSQUATDotProduct(register const Quaternion *p, register const Quaternion *q)
    {
        register f32 dp, qxy, pxy, qzw, pzw;

        __asm
        {
        psq_l       pxy, 0(p), 0, 0
        psq_l       qxy, 0(q), 0, 0
        ps_mul      dp, pxy, qxy
        
        psq_l       pzw, 8(p), 0, 0
        psq_l       qzw, 8(q), 0, 0
        ps_madd     dp, pzw, qzw, dp
        
        ps_sum0     dp, dp, dp, dp
        }

        return dp;
    }
}

#endif // !JMATH_H
