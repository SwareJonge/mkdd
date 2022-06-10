#ifndef JMATH_H
#define JMATH_H

#include <types.h>
#include "JMATrigonometric.h"

namespace JMath {
    class TRandom_fast_
    {
    public:
        TRandom_fast_(u32 num);
        inline u32 get();

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

        inline setSeed(u32 Seed);

    private:
        u32 seed;
    };
}

#endif // !JMATH_H
