#ifndef JAUDIO_JASCRITICALSECTION_H
#define JAUDIO_JASCRITICALSECTION_H

#include "dolphin/os.h"

class JASCriticalSection
{
public:
    JASCriticalSection() { mInterruptState = OSDisableInterrupts(); };
    ~JASCriticalSection() { OSRestoreInterrupts(mInterruptState); };

private:
    u32 mInterruptState;
};

#endif