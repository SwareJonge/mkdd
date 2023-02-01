#ifndef AWARD2D_H
#define AWARD2D_H

#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

class Award2D {
public:
    Award2D(JKRHeap *);
    void still();

    u8 _0[0x58];
};

#endif