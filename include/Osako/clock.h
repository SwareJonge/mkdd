#ifndef CLOCK_H
#define CLOCK_H
#include <dolphin/os.h>

class Clock
{
public:

    // Fabricated Inlines
    static u32 getTimeDiff(u32 other) { return OSDiffTick(msTime, other); }
    static f32 calcFrameTime(u32 other)
    {
        return getTimeDiff(other) / (f32)Clock::mscTimePerSec;
    }   

    static void move();
private:
    static int msTime;
    static int msPrevFrameTime;
    static u32 msPrevTick;
    static const s32 mscTimePerSec;
};

#endif
