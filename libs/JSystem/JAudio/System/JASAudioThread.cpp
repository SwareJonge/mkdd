#include "JSystem/JAudio/System/JASAudioThread.h"
#include "JSystem/JAudio/System/JASChannel.h"
#include "JSystem/JAudio/System/JASDriver.h"
#include "JSystem/JAudio/System/JASDsp.h"
#include "JSystem/JAudio/System/JASGadget.h"
#include "JSystem/JAudio/System/JASHeap.h"
#include "JSystem/JAudio/System/JASProbe.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRThread.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/dsp.h"
#include "dolphin/os/OSFastCast.h"
#include "dolphin/os/OSMessage.h"
#include "dolphin/os/OSThread.h"
#include "types.h"

template <>
JASMemPool_MultiThreaded<JASChannel> JASPoolAllocObject_MultiThreaded<JASChannel>::memPool_;

volatile int JASAudioThread::snIntCount;

JASAudioThread::JASAudioThread(int prio, int msgCount, u32 stackSize) : JKRThread(JASDram, stackSize, msgCount, prio), JASGlobalInstance<JASAudioThread>(true)  {
    sbPauseFlag = false;
    OSInitThreadQueue(&sThreadQueue);
}

void JASAudioThread::create(s32 prio) {
#line 42
    JASAudioThread *pAudioThread = new (JASDram, 0) JASAudioThread(prio, 0x10, 0x1000);
    JUT_ASSERT(pAudioThread);
    JKRHeap *pCurrentHeap = JKRGetSystemHeap();
    JUT_ASSERT(pCurrentHeap);
    pAudioThread->setCurrentHeap(pCurrentHeap);
    pAudioThread->resume();
}

void JASAudioThread::stop() {
    jamMessageBlock((OSMessage)2);
}

void *JASAudioThread::run() {
    OSInitFastCast();
    JASDriver::initAI(DMACallback);
    JASDsp::boot(DSPCallback);
    JASDsp::initBuffer();
    JASDSPChannel::initAll();
    JASPoolAllocObject_MultiThreaded<JASChannel>::newMemPool(0x48);
    JASDriver::startDMA();

    do {
        OSMessage msg = waitMessageBlock();

        switch ((int)msg) {
        case 0: {
            if (sbPauseFlag) {
                //JUT_ASSERT_MSG(false, "AUDIO THREAD PAUSED\n");
                JASDriver::stopDMA();
                OSSleepThread(&sThreadQueue);
            }
            JASDriver::updateDac();
            JASDriver::updateDacCallback();
            break;
        }
        case 1: {
#line 119
            JUT_ASSERT(snIntCount != 0);
            snIntCount--;
            if (snIntCount == 0) {
                JASProbe::stop(7);
                JASDriver::finishDSPFrame();
            }
            else {
                JASProbe::start(2, "SFR_DSP");
                JASDriver::updateDSP();
                JASProbe::stop(2);
            }
            break;
        }
        case 2: {
            JASDriver::stopDMA();
            OSExitThread(NULL);
            break;
        }
        default: {
            //JUT_ASSERT_MSG(false, "AUDIO THREAD INVALID MESSAGE\n");
            break;
        }
        }

    } while (true);
}

void JASAudioThread::DMACallback() {
#line 152
    JASAudioThread* pAudioThread = getInstance();
    JUT_ASSERT(pAudioThread);

    JASProbe::stop(4);
	JASProbe::start(4, "UPDATE-DAC");	
    BOOL res = pAudioThread->sendMessage((OSMessage)AUDIOMSG_DMA);
    if (res == FALSE) {
        JUT_WARNING_F1("%s", "----- DMACallback : Can't send DAC_SYNC message\n");
    }
}

void JASAudioThread::DSPCallback(void*) {
#line 169
    JASAudioThread* pAudioThread = getInstance();
    JUT_ASSERT(pAudioThread);

	while (DSPCheckMailFromDSP() == 0) { }
	u32 mail = DSPReadMailFromDSP();
    if (mail >> 0x10 != 0xF355) {
#line 182
        JUT_WARNING_F(false, "DSP Mail format error %x\n", mail);
    }
    else if ((mail & 0xFF00) == 0xFF00) {
        BOOL res = pAudioThread->sendMessage((OSMessage)AUDIOMSG_DSP);
        if (res == FALSE) {
#line 190
            JUT_WARNING_F1("%s", "----- syncDSP : Send Miss\n");
        }
    } else {
        JASDsp::finishWork(mail);
    }
}

void JASAudioThread::bootDSP() {
    JASDsp::boot(DSPCallback);
}
