#ifndef SYSTEM_H
#define SYSTEM_H

#include "dolphin/OS.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPrint.h"
#include "JSystem/JFrameWork/JFWDisplay.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRAssertHeap.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JKernel/JKRThread.h"
#include "types.h"

namespace System
{
    extern JKRTask::RequestCallback startAudioTask;
    void init();
    void reset();
    void changeProgressive();
    void changeMovieRenderMode();
    void changeNormalRenderMode();
    extern OSErrorHandler callbackException;
    void haltRumble();
    void run();
    // Unused functions however i'm sure these get auto inlined in run()
    void checkDVDState();
    void beginFrame();
    void endRender();
    void endFrame();

    extern JFWDisplay *mspDisplay;
    extern JKRHeap *mspAppHeap; // JKRExpHeap
    extern int msDvdState;
    extern int msRenderMode;
    extern JKRTask *mspSendTask;
    extern JKRTask *mspRecvTask;
    extern JKRTask *mspAramTask;
    extern JKRTask *mspLoTask;
    extern J2DPrint *mspJ2DPrint;
    extern J2DOrthoGraph *mspJ2DOrtho;
    extern JKRSolidHeap *mspAudioHeap;
    // Unused
    // extern int msPerfMode;

    inline JFWDisplay *getDisplay() { return mspDisplay; }
    inline JKRHeap *getAppHeap() { return mspAppHeap; }
    inline J2DPrint *getJ2DPrint() { return mspJ2DPrint; }
    inline J2DOrthoGraph *getJ2DOrtho() { return mspJ2DOrtho; }
};

#endif