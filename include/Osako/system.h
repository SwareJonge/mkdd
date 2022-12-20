#ifndef SYSTEM_H
#define SYSTEM_H

#include "Dolphin/OS.h"
#include "JSystem/JKernel/JKRHeap.h"

#include "types.h"

class JFWDisplay;
class JKRTask;
class J2DOrthoGraph;
class J2DPrint;

// Make this a class?
namespace System {
    void startAudioTask(void* p1);
    void init();
    void reset();
    void changeProgressive();
    void changeMovieRenderMode();
    void changeNormalRenderMode();
    void callbackException(u16, OSContext, u32, u32);
    void haltRumble();
    void run();

    extern JFWDisplay * mspDisplay;
    extern JKRHeap * mspAppHeap;
    extern int msDvdState;
    extern int msRenderMode;
    extern JKRTask * mspSendTask;
    extern JKRTask * mspRecvTask;
    extern JKRTask * mspAramTask;
    extern JKRTask * mspLoTask;
    extern J2DPrint * mspJ2DPrint;
    extern J2DOrthoGraph * mspJ2DOrtho;
    extern JKRHeap * mspAudioHeap;
}

#endif