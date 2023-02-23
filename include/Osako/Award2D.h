#ifndef AWARD2D_H
#define AWARD2D_H

#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

class Award2D {
public:
    Award2D(JKRHeap *);
    void calc();
    void still() {
        _10 = 0;
    }

    u8 _00[0x10];
    u32 _10;
    u8 _14[0x58 - 0x14];
};

#endif