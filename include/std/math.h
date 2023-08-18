#ifndef STD_MATH_H
#define STD_MATH_H

// TODO: Get rid of this file, it should use the one from MSL_C instead

#include <math.h>
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/math_api.h"
#include "types.h"

namespace std
{
    extern inline f32 sqrtf(f32 x)
    {
        const double _half = .5f;
        const double _three = 3.0f;
        if (x > 0.0f)
        {
            double xd = (double)x;
            double guess = __frsqrte(xd);                          // returns an approximation to
            guess = _half * guess * (_three - guess * guess * xd); // now have 12 sig bits
            guess = _half * guess * (_three - guess * guess * xd); // now have 24 sig bits
            guess = _half * guess * (_three - guess * guess * xd); // now have 32 sig bits
            return (float)(xd * guess);
        }
        else if (x < 0.0)
        {
            return NAN;
        }
        else if (isnan(x))
        {
            return NAN;
        }
        else {
            return x;
        }        
    }
    extern inline f32 inv_sqrtf(f32 x) {
        return 1.0f / sqrtf(x);

    }
}

#endif