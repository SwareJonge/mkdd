#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <dolphin/mtx.h>


inline void QuaternionReset(Quaternion *pDest)
{
    const Quaternion q = {0.0f, 0.0f, 0.0f, 1.0f};
    *pDest = q;
}


#endif