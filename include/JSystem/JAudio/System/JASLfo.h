#ifndef JASLFO_H
#define JASLFO_H

#include "types.h"

struct JASLfo
{
    JASLfo();
    f32 getValue() const;
    void incCounter(f32);
    void resetCounter();

    void setDepth(f32 depth) { mDepth = depth; }
    void setPitch(f32 pitch) { mPitch = pitch; }
    void setDelay(u16 delay) { mDelay = delay; }

    static void updateFreeRun(f32 rate) { sFreeRunLfo.incCounter(rate); }

    static JASLfo sFreeRunLfo;

    u32 _0;       // 00
    u32 _4;       // 04
    f32 _8;       // 08
    f32 mDepth;   // 0C
    f32 mPitch;   // 10
    u16 mDelay;   // 14
    u16 mCounter; // 16
};

#endif
