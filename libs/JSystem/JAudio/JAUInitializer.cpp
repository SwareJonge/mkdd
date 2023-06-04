#include <JSystem/JAudio/JAUInitializer.h>
#include <JSystem/JKernel/JKRAram.h>
#include <JSystem/JUtility/JUTDbg.h>

JAU_JASInitializer::JAU_JASInitializer()
{
    mAudioMemory = 0;
    mAudioMemSize = 0;
    mDvdThreadID = -1;
    mAudioThreadID = -1;
    mHeapSize = 0x1000;
    mDvdThreadPrio = 3;
    mAudioThreadPrio = 2;
    mTrackPools = 128;
    mDSPLevel = 1.0f;
    mChannelMax = 2;
    mChannelPools = 0;
    mDir = nullptr;
}

void JAU_JASInitializer::initJASystem(JKRSolidHeap *heap) {
    if (JASAudioThread::getThreadPointer() != nullptr) {
#line 48
        JUT_CRITICAL_WARNING_F("%s", "Though initialized JASAudioThread, called JAU_JASInitializer::initJASystem()\n");
    }
    else {
        if (JASDvd::getThreadPointer() != nullptr) {
            JUT_CRITICAL_WARNING_F("%s", "Though initialized JASAudioThread, called JAU_JASInitializer::initJASystem()\n");
        }
        else {
            JASKernel::setupRootHeap(heap, mHeapSize);
            if(mAudioMemory == 0) {
                mAudioMemory = JKRAram::getManager()->getAudioMemory();
            }
            if(mAudioMemSize == 0) {
                mAudioMemSize = JKRAram::getManager()->getAudioMemSize();
            }
            JASKernel::setupAramHeap(mAudioMemory, mAudioMemSize);
        }
    }
}