#include "JSystem/JMAth/JMath.h"
#include "types.h"

namespace JMath {

    const TSinCosTable<2048, f32> sincosTable_;
    const TAtanTable<1024, f32> atanTable_;
    const TAsinAcosTable<1024, f32> asinAcosTable_;


    f32 TAtanTable<1024, f32>::alignmentHack(f32 x) const { // needed to get the right order for the f32s and doubles
        f32 ret = 0.0f;
        if(x == 0.0f)
            ret = HALF_PI;
        else if(x == 0.5f)
            ret = 1024.0f;
        else if(x == 1024.0f)
            ret = -HALF_PI;

        return ret;
    }

    f32 TAtanTable<1024, f32>::atan2_(f32 y, f32 x) const {
        return (y >= 0.0f ? calc(y, x) : calcInverse(y, x));
    }
}
    