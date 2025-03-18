#include "JSystem/JAudio/JAUInitializer.h"
#include "JSystem/JAudio/Interface/JAIStream.h"
#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JAudio/Interface/JAISeq.h"
#include "JSystem/JAudio/System/JASAudioThread.h"
#include "JSystem/JAudio/System/JASDvd.h"
#include "JSystem/JAudio/System/JASDriver.h"
#include "JSystem/JAudio/System/JASHeapCtrl.h"
#include "JSystem/JAudio/System/JASKernel.h"
#include "JSystem/JAudio/System/JASWaveArcLoader.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JUtility/JUTDbg.h"


#include "JSystem/JAudio/JASFakeMatch11.h"

JAU_JASInitializer::JAU_JASInitializer()
{
    mAudioMemory = 0;
    mAudioMemSize = 0;
    mDvdThreadID = -1;
    mAudioThreadID = -1;
    mHeapSize = 0x1000;
    mDvdThreadPrio = 3;
    mAudioThreadPrio = 2;
    mTrackPoolCount = 128;
    mDSPLevel = 1.0f;
    mBlockSize = 0x2760;
    mChannelMax = 2;
    mChannelPoolCount = 0;
    mDir = NULL;
}

void JAU_JASInitializer::initJASystem(JKRSolidHeap *heap)
{
    if (JASAudioThread::getThreadPointer() != NULL)
    {
#line 48
        JUT_WARNING_F2("%s", "Though initialized JASAudioThread, called JAU_JASInitializer::initJASystem()\n");
    }
    else if (JASDvd::getThreadPointer() != NULL)
    {
        JUT_WARNING_F2("%s", "Though initialized JASDvd, called JAU_JASInitializer::initJASystem()\n");
    }
    else
    {
        JASKernel::setupRootHeap(heap, mHeapSize);
        if (mAudioMemory == 0)
        {
            mAudioMemory = JKRAram::getManager()->getAudioMemory();
        }
        if (mAudioMemSize == 0)
        {
            mAudioMemSize = JKRAram::getManager()->getAudioMemSize();
        }
        JASKernel::setupAramHeap(mAudioMemory, mAudioMemSize);

        JASPoolAllocObject_MultiThreaded<JASTrack>::newMemPool(mTrackPoolCount);
        if(mChannelPoolCount > 0) {
            JASPoolAllocObject_MultiThreaded<JASTrack::TChannelMgr>::newMemPool(mChannelPoolCount);
        }
        JASDvd::createThread(mDvdThreadPrio, 0x80, 0x1000);
        JASAudioThread::create(mAudioThreadPrio);

        JKRThreadSwitch *threadSwitch = JKRThreadSwitch::getManager();
        if (threadSwitch)
        {
            if(mDvdThreadID >= 0) {
                threadSwitch->enter(JASDvd::getThreadPointer(), mDvdThreadID);
            }
            if (mAudioThreadID >= 0)
            {
                threadSwitch->enter(JASAudioThread::getThreadPointer(), mAudioThreadID);
            }
        }
        JASDriver::setDSPLevel(mDSPLevel);
        JASAramStream::initSystem(mBlockSize, mChannelMax);
    }

    if(mDir) {
        JASWaveArcLoader::setCurrentDir(mDir);
    }
    //JASDriver::setOutputMode(1);
}

JAU_JAIInitializer::JAU_JAIInitializer()
{
    mSeCount = 100;
    mSeqCount = 4;
    mStreamCount = 2;
    mChildCount = 16;
}

void JAU_JAIInitializer::initJAInterface()
{
    int oldSize = JASDram->getFreeSize();
    JASPoolAllocObject<JAIStream>::newMemPool(mStreamCount);
    JASPoolAllocObject<JAISeq>::newMemPool(mSeqCount);
    JASPoolAllocObject<JAISe>::newMemPool(mSeCount);
    JASPoolAllocObject<JAISoundChild>::newMemPool(mChildCount);
    int newSize = JASDram->getFreeSize();
    JUT_REPORT_MSG("JAU_JAIInitializer uses %d bytes\n", oldSize - newSize);
}

void JAUReportPoolCount() 
{
    // something something cur/Count
    JUT_REPORT_MSG("JASTrack %d/%d : JASTrack::TChannelMgr %d/%d\n");
    JUT_REPORT_MSG("JAISe %d/%d : JAISeq %d/%d : JAIStream %d/%d : JAISoundChild %d/%d\n");
}
