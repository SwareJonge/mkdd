#include "types.h"
#include <JSystem/JAudio/System/JASDvd.h>
#include <JSystem/JAudio/JASHeap.h>
#include <JSystem/JUtility/JUTDbg.h>

namespace JASDvd
{
    static JASTaskThread *sThread;

    JASTaskThread *getThreadPointer() { return sThread; }

    bool createThread(s32 threadPriority, int msgCount, u32 stackSize)
    {
        // check if createThread already has been called
        if (sThread != 0) // could be conditional JUT_CRITICAL_WARNING_F, who knows
        {
#line 32
            JUT_CRITICAL_WARNING_F("%s", "JASDvd::createThread が２回呼び出されました");
            return false;
        }
        sThread = new (JASDram, 0) JASTaskThread(threadPriority, msgCount, stackSize);
        JUT_ASSERT(36, sThread);
        JKRHeap *pCurrentHeap = JKRGetSystemHeap();
        JUT_ASSERT(38, pCurrentHeap);
        sThread->setCurrentHeap(pCurrentHeap);
        sThread->resume();
        return true;
    }
}
