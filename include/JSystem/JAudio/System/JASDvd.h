#ifndef JSYSTEM_JASDVD_H
#define JSYSTEM_JASDVD_H
#include "types.h"
#include <JSystem/JAudio/JASTaskThread.h>

namespace JASDvd {
    bool createThread(s32 threadPriority, int msgCount, u32 stackSize);
    JASTaskThread *getThreadPointer();
};

#endif
