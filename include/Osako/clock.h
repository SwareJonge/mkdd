#ifndef CLOCK_H
#define CLOCK_H
#include "dolphin/OS.h"

class Clock {
    public:
    static void move();
    static int msTime;
    static int msPrevFrameTime;
    static OSTick msPrevTick;
    static const u32 mscTimePerSec;
};

#endif