#ifndef STD_MATH_H
#define STD_MATH_H

// TODO: Get rid of this file, it should use the one from MSL_C instead

#include <math.h>
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/math_api.h"
#include "types.h"

#ifdef __cplusplus
namespace std
{
#endif

#if __MWERKS__ > 0x2301
extern inline float sqrtf(float x)
{
    static const double _half = .5f;
    static const double _three = 3.0f;
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
    else
    {
        return x;
    }
}
#else
extern inline float sqrtf(float x)
{
    static const double _half = .5;
    static const double _three = 3.0;
    volatile float y;
    if (x > 0.0f)
    {
        double guess = __frsqrte((double)x);                  // returns an approximation to
        guess = _half * guess * (_three - guess * guess * x); // now have 12 sig bits
        guess = _half * guess * (_three - guess * guess * x); // now have 24 sig bits
        guess = _half * guess * (_three - guess * guess * x); // now have 32 sig bits
        y = (float)(x * guess);
        return y;
    }
    return x;
}
#endif

extern inline float inv_sqrtf(float x) {
    return 1.0f / sqrtf(x);
}

#ifdef __cplusplus
extern inline float fabsf(float x) { return ::fabs(x); }
extern inline float fabs(float x) { return fabsf(x); }

extern inline float atan2f(float y, float x) { return ::atan2(y, x); }
extern inline float cosf(float x) { return ::cos(x); }
extern inline float sinf(float x) { return ::sin(x); }
extern inline float tanf(float x) { return ::tan(x); }
extern inline float tan(float x) { return tanf(x); }
#endif
#ifdef __cplusplus
}
#endif

#endif
