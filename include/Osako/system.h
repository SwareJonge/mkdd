#ifndef SYSTEM_H
#define SYSTEM_H

#include "dolphin/OS.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPrint.h"
#include "JSystem/JFrameWork/JFWDisplay.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRAssertHeap.h"
#include "types.h"

class JKRTask;

namespace System {
    void startAudioTask(void *p1);
    void init();
    void reset();
    void changeProgressive();
    void changeMovieRenderMode();
    void changeNormalRenderMode();
    void callbackException(u16, OSContext, u32, u32);
    void haltRumble();
    void run();

    extern JFWDisplay *mspDisplay;
    extern JKRAssertHeap *mspAppHeap;
    extern int msDvdState;
    extern int msRenderMode;
    extern JKRTask *mspSendTask;
    extern JKRTask *mspRecvTask;
    extern JKRTask *mspAramTask;
    extern JKRTask *mspLoTask;
    extern J2DPrint *mspJ2DPrint;
    extern J2DOrthoGraph *mspJ2DOrtho;
    extern JKRHeap *mspAudioHeap; // JKRSolidHeap

    inline JFWDisplay *getDisplay() { return mspDisplay; }
    inline JKRAssertHeap *getAppHeap() { return mspAppHeap; }
    inline J2DPrint *getJ2DPrint() { return mspJ2DPrint; }
    inline J2DOrthoGraph *getJ2DOrtho() { return mspJ2DOrtho; }
};

#endif