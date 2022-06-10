#include <JSystem/JMath/JMath.h>

namespace JMath {
    TRandom_fast_::TRandom_fast_(u32 num = 0) {
        random = num;
    }

    u32 TRandom_fast_::get() {
        seed = (seed * 0x19660d) + 0x3c6ef35f;
        return seed;
    }

    void TRandom_fast_::setSeed(u32 Seed) {
        seed = Seed; 
    }
}