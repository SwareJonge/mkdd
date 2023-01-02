#ifndef _JSYSTEM_JUT_JUTGRAPHFIFO_H
#define _JSYSTEM_JUT_JUTGRAPHFIFO_H

#include "types.h"
#include "Dolphin/gx.h"

struct JUTGraphFifo
{
    static JUTGraphFifo *create(u32 p1)
    {
        return new JUTGraphFifo(p1);
    }

    JUTGraphFifo(u32);

    virtual ~JUTGraphFifo(); // _08

    void becomeCurrent();
    void setBreakPt();

    static JUTGraphFifo *sCurrentFifo;
    static GXBool mGpStatus[5];

    // VTBL _00
    GXFifoObj *m_fifo; // _04
    void *_08;         // _08
    u32 _0C;           // _0C
    u8 _10[0xC];       // _10

    static bool sInitiated;
};

#endif