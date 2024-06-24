#include "types.h"
#include <JSystem/JAudio/System/JASDvd.h>
#include <JSystem/JAudio/System/JASHeap.h>
#include <JSystem/JUtility/JUTDbg.h>

namespace JASDvd
{
    static JASTaskThread *sThread;

    JASTaskThread *getThreadPointer() { return sThread; }

    bool createThread(s32 threadPriority, int msgCount, u32 stackSize)
    {
        // check if createThread already has been called
        if (sThread != 0)
        {
#line 32
            JUT_CRITICAL_WARNING_F("%s", "JASDvd::createThread ‚ª‚Q‰ñŒÄ‚Ño‚³‚ê‚Ü‚µ‚½");
            return false;
        }
        sThread = new (JASDram, 0) JASTaskThread(threadPriority, msgCount, stackSize);
        JUT_ASSERT(sThread);
        JKRHeap *pCurrentHeap = JKRGetSystemHeap();
        JUT_ASSERT(pCurrentHeap);
        sThread->setCurrentHeap(pCurrentHeap);
        sThread->resume();
        return true;
    }
}
