#ifndef STAFFROLL2D_H
#define STAFFROLL2D_H

#include "kartLocale.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

class StaffRoll2D {
public:
    StaffRoll2D(JKRHeap *, Language, VideoMode, int);
    void calc();
    void reset();
    u8 _0[0x4598]; // beeg
};

#endif