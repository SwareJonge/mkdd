#ifndef JASAUDIOTHREAD_H
#define JASAUDIOTHREAD_H

#include "JSystem/JAudio/System/JASGadget.h"
#include "JSystem/JKernel/JKRThread.h"

enum JASAudioMessage
{
    AUDIOMSG_DMA = 0,
    AUDIOMSG_DSP = 1,
    AUDIOMSG_STOP = 2,
};

struct JASAudioThread : public JKRThread, public JASGlobalInstance<JASAudioThread>
{
    JASAudioThread(int, int, u32);
    static void create(s32);
    static void bootDSP();
    void stop();
    virtual void *run();
    static void DMACallback();
    static void DSPCallback(void *);
    virtual ~JASAudioThread() {}

    static JASAudioThread *getThreadPointer() { return getInstance(); }
    static int getDSPSyncCount() { return snIntCount; }
    static void setDSPSyncCount(u32 count) { snIntCount = count; }

    OSThreadQueue sThreadQueue; // 7c
    bool sbPauseFlag;           // b4, type unsure

    static volatile int snIntCount; // type unsure
};

#endif /* JASAUDIOTHREAD_H */
