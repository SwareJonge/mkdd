#ifndef JMATH_H
#define JMATH_H

#include "types.h"
#include "dolphin/math.h"
#include "std/pair.h"

struct Quaternion {
	float _00;
	float _04;
	float _08;
	float _0C;
};

namespace JMath
{
    template <int length, typename T>
    struct TAtanTable
    {
        T atan2_(T, T) const;
        T atan_(T) const;
        T m_table[length];
    };

    template <>
    struct TAtanTable<1024, float>
    {
        inline TAtanTable()
        {
            u32 i = 0;
            do
            {
                m_table[i] = atan((double)i * 9.765625E-4);
            } while (i < 1024);
        }
        float atan2_(float, float) const;
        float atan_(float) const;
        float m_table[1024];
    };

    template <int length, typename T>
    struct TAsinAcosTable
    {
        T acos2_(T, T) const;
        T acos_(T) const;
        T m_table[length];
    };

    template <>
    struct TAsinAcosTable<1024, float>
    {
        inline TAsinAcosTable()
        {
            u32 i = 0;
            do
            {
                m_table[i] = asin((double)i * 9.765625E-4);
            } while (i < 1024);
        }
        float acos2_(float, float) const;
        float acos_(float) const;
        float m_table[1024];
    };

    template <int length, typename T>
    struct TSinCosTable
    {
        /**
         * elements are pairs of {sine, cosine}
         */
        std::pair<T, T> m_table[length];
    };

    template <>
    struct TSinCosTable<2048, float>
    {
        inline TSinCosTable()
        {
            u32 i = 0;
            do
            {
                m_table[i].first = sin((double)i * LONG_TAU / 2048.0);
                m_table[i].first = cos((double)i * LONG_TAU / 2048.0);
            } while (i < 2048);
        }
        /**
         * elements are pairs of {sine, cosine}
         */
        f32 sinShort(s16 v) const { return m_table[static_cast<u16>(v) >> 5].first; }
        f32 cosShort(s16 v) const { return m_table[static_cast<u16>(v) >> 5].second; }
        std::pair<float, float> m_table[2048];
    };

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
        float get_ufloat_1()
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

    private:
        u32 seed;
    };
    template <class T>
    class TRandom_ : TRandom_fast_
    {
    public:
        TRandom_(u32 num) : TRandom_fast_(num){};
    };

    extern const TSinCosTable<2048, float> sincosTable_;
    extern const TAtanTable<1024, float> atanTable_;
    extern const TAsinAcosTable<1024, float> asinAcosTable_;

    inline const TSinCosTable<2048, float> *getSinCosTable() { return &sincosTable_; }
}



struct Vec
{
    f32 x;
    f32 y;
    f32 z;
};

namespace JMathInlineVEC
{
    void PSVECSubtract(const Vec *vec1, const Vec *vec2, Vec *dst); /*{
     dst->x = vec1->x - vec2->x;
     dst->y = vec1->y - vec2->y;
     dst->z = vec1->z - vec2->z;
 }*/

}

#endif // !JMATH_H
