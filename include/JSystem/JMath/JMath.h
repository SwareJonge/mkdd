#ifndef JMATH_H
#define JMATH_H

#include "types.h"
#include "JSystem/JMath/JMATrigonometric.h"

namespace JMath {
    TSinCosTable sincosTable_;

    class TRandom_fast_
    {
    public:
        TRandom_fast_(u32 num);
        u32 get() {
            seed = (seed * 0x19660d) + 0x3c6ef35f;
            return seed;
        }

        // from TP decomp
        float get_ufloat_1() {
            // !@bug UB: in C++ it's not legal to read from an union member other
            // than the last one that was written to.
            union {
                f32 f;
                u32 s;
            } out;
            out.s = (this->get() >> 9) | 0x3f800000;
            return out.f - 1;
        }

        void setSeed(u32 Seed) {
            seed = Seed;
        }

    private:
        u32 seed;
    };
    template <class T>
    class TRandom_ : TRandom_fast_ {
    public:
        TRandom_(u32 num) : TRandom_fast_(num) {};
    };
}

#endif // !JMATH_H
