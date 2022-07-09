#ifndef SYSTEM_H
#define SYSTEM_H

#include "Dolphin/OS.h"
#include "JSystem/JKernel/JKRHeap.h"

#include "types.h"

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

    /*JFWDisplay * mspDisplay;
    JKRHeap * mspAppHeap;
    int msDvdState;
    int msRenderMode;
    JKRTask * mspSendTask;
    JKRTask * mspRecvTask;
    JKRTask * mspAramTask;
    JKRTask * mspLoTask;
    J2DPrint * mspJ2DPrint;
    J2DOrthoGraph * mspJ2DOrtho;
    JKRHeap * mspAudioHeap;*/

}

#endif