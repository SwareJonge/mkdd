#ifndef JASLFO_H
#define JASLFO_H

#include "types.h"

struct JASLfo
{
    JASLfo();
    f32 getValue() const;
    void incCounter(f32);
    void resetCounter();

    void setDepth(f32 param_0) { mDepth = param_0; }
    void setPitch(f32 param_0) { mPitch = param_0; }
    void setDelay(u16 param_0) { mDelay = param_0; }

    static void updateFreeRun(f32 param_0) { sFreeRunLfo.incCounter(param_0); }

    static JASLfo sFreeRunLfo;

    u32 _0;     // 00
    u32 _4;     // 04
    f32 _8;     // 08
    f32 mDepth; // 0C
    f32 mPitch; // 10
    u16 mDelay; // 14
    u16 _16;    // 16
};

#endif