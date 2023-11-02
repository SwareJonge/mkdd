#include <dolphin/os.h>
#include "Osako/clock.h"

int Clock::msTime;
int Clock::msPrevFrameTime;
OSTick Clock::msPrevTick;
const s32 Clock::mscTimePerSec = 300;

void Clock::move() {
    OSTick curTick = OSGetTick();
    if (msPrevTick != 0) {
        msPrevFrameTime = (f32)(mscTimePerSec) * OSDiffTick(curTick, msPrevTick) / OS_TIMER_CLOCK;
        msTime += msPrevFrameTime;
    }
    msPrevTick = curTick;
}
