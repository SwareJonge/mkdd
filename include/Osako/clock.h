#ifndef CLOCK_H
#define CLOCK_H
#include <dolphin/os.h>

class Clock {
    public:
    static void move();
    static int msTime;
    static int msPrevFrameTime;
    static OSTick msPrevTick;
    static const u32 mscTimePerSec;
};

#endif
