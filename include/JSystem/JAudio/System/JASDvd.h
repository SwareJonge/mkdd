#ifndef JAUDIO_JASDVD_H
#define JAUDIO_JASDVD_H
#include "types.h"
#include <JSystem/JAudio/System/JASTaskThread.h>

namespace JASDvd {
    bool createThread(s32 threadPriority, int msgCount, u32 stackSize);
    JASTaskThread *getThreadPointer();
};

#endif
