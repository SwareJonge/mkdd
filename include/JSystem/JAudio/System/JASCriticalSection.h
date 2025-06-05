#ifndef JAUDIO_JASCRITICALSECTION_H
#define JAUDIO_JASCRITICALSECTION_H

#include "dolphin/os.h"

// starts a critical section scope
#define JAS_CS_START { \
    JASCriticalSection cs;

// ends a critical section scope
#define JAS_CS_END }

#define JAS_CS(code) \
    JAS_CS_START \
    code \
    JAS_CS_END

class JASCriticalSection
{
public:
    JASCriticalSection() { mInterruptState = OSDisableInterrupts(); };
    ~JASCriticalSection() { OSRestoreInterrupts(mInterruptState); };

private:
    BOOL mInterruptState;
};

#endif
