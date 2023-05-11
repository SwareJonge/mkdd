#ifndef JGEOMETRY_UTIL_H
#define JGEOMETRY_UTIL_H

#include <math.h>
#include <JSystem/JMath/JMath.h>
#include "types.h"

namespace JGeometry {
    template <typename T>
    class TUtil
    {
    public:
        static f32 abs(f32 x) {
            return __fabs(x);
        }
        static f32 atan2(f32 y, f32 x) { return JMAAtan2Radian(y, x); }
        static const f32 epsilon() { return 32.0f * FLT_EPSILON; }

        static int epsilonEquals(T a1, T a2, T a3)
        {
            int ret = 0;

            a1 -= a2;

            if (-a3 <= a1 && a1 <= a3)
            {
                ret = 1;
            }

            return ret;
        }

        static T sqrt(T val)
        {
            if (val <= 0.0f)
                return 0.0f;


            float outVal = __frsqrte(val);
            return (val * (0.5f * outVal * (3.0f - (val * (outVal * outVal)))));
        }

        static T inv_sqrt(T number)
        {
            if (number <= 0.0f)
                return number;

            const f32 threehalfs = 3.0f;
            f32 y = __frsqrte(number);
            y = 0.5f * y * (threehalfs - (number * (y * y)));

            return y;
        }

        // TODO: probably uses seomthing from TSinCosTable
        /*static f32 asin(f32 val) NO_INLINE
        {
            if (val >= 1.0f)
            {
                return 1.5707964f;
            }

            if (val <= -1.0f)
            {
                return -1.5707964f;
            }

            if (val < 0.0f)
            {
                return -flt_80630CA0[(u32)(1023.5f * -val)];
            }

            return flt_80630CA0[(u32)(1023.5f * val)];
        }*/

        /*static f32 PI() NO_INLINE
        {
            return 3.1415927f;
        }*/

        static T clamp(T p1, T p2, T p3);
    };
}

#endif
