#ifndef STD_MATH_H
#define STD_MATH_H

#include <math.h>
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/math_api.h"
#include "types.h"

namespace std
{
    inline f32 sqrtf(f32 x)
    {
        if (x > 0.0f)
        {
            const double _half = .5;
            const double _three = 3.0;
            double guess = __frsqrte((double)x);                  // returns an approximation to
            guess = _half * guess * (_three - guess * guess * x); // now have 12 sig bits
            guess = _half * guess * (_three - guess * guess * x); // now have 24 sig bits
            guess = _half * guess * (_three - guess * guess * x); // now have 32 sig bits
            return (x * guess);
        }
        else if (x < 0.0)
        {
            return NAN;
        }
        else if (__fpclassifyf(x) == 1)
        {
            return NAN;
        }
        else {
            return x;
        }        
    }
    inline f32 inv_sqrtf(f32 x) {
        return 1.0f / sqrtf(x);

    }
}

#endif