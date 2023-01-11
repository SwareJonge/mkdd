#ifndef SYSTEM_H
#define SYSTEM_H

#include "Dolphin/OS.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPrint.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRAssertHeap.h"


#include "types.h"

class JFWDisplay;
class JKRTask;

class System {
    static JFWDisplay *mspDisplay;
    static JKRAssertHeap *mspAppHeap;
    static int msDvdState;
    static int msRenderMode;
    static JKRTask *mspSendTask;
    static JKRTask *mspRecvTask;
    static JKRTask *mspAramTask;
    static JKRTask *mspLoTask;
    static J2DPrint *mspJ2DPrint;
    static J2DOrthoGraph *mspJ2DOrtho;
    static JKRHeap *mspAudioHeap; // JKRSolidHeap
public:
    static void startAudioTask(void *p1);
    static void init();
    static void reset();
    static void changeProgressive();
    static void changeMovieRenderMode();
    static void changeNormalRenderMode();
    static void callbackException(u16, OSContext, u32, u32);
    static void haltRumble();
    static void run();
    static JFWDisplay *getDisplay() {
        return mspDisplay;
    }
    static JKRAssertHeap *getAppHeap() {
        return mspAppHeap;
    }
    static J2DPrint *getJ2DPrint() {
        return mspJ2DPrint;
    } 
    static J2DOrthoGraph *getJ2DOrtho() {
        return mspJ2DOrtho;
    }
};

#endif