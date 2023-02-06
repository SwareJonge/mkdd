#ifndef SYSTEM_H
#define SYSTEM_H

#include "dolphin/OS.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPrint.h"
#include "JSystem/JUtility/JUTException.h"
#include "JSystem/JFrameWork/JFWDisplay.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRAssertHeap.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JKernel/JKRThread.h"
#include "types.h"

namespace System
{    
    void init();
    void reset();
    void changeProgressive();
    void changeMovieRenderMode();
    void changeNormalRenderMode();    
    void haltRumble();
    void run();
    // Unused functions however i'm sure these get auto inlined in run()
    void checkDVDState();
    void beginFrame();
    void endRender();
    void endFrame();

    // callbacks
    extern JKRTask::RequestCallback startAudioTask;
    extern JUTErrorHandler callbackException;

    f32 get2DVpX();
    f32 get2DVpY();
    f32 get2DVpW();
    f32 get2DVpH();
    f32 get3DVpX();
    f32 get3DVpY();
    f32 get3DVpW();
    f32 get3DVpH();
    f32 get3DVpDiv2X(u8);
    f32 get3DVpDiv2Y(u8);
    f32 get3DVpDiv2W(u8);
    f32 get3DVpDiv2H(u8);
    f32 get3DVpDiv4X(u8);
    f32 get3DVpDiv4Y(u8);
    f32 get3DVpDiv4W(u8);
    f32 get3DVpDiv4H(u8);
    f32 get3DVpSubX();
    f32 get3DVpSubY();
    f32 get3DVpSubW();
    f32 get3DVpSubH();
    u32 get2DScisX();
    u32 get2DScisY();
    u32 get2DScisW();
    u32 get2DScisH();
    u32 get3DScisX();
    u32 get3DScisY();
    u32 get3DScisW();
    u32 get3DScisH();
    u32 get3DAwardScisX();
    u32 get3DAwardScisY();
    u32 get3DAwardScisW();
    u32 get3DAwardScisH();
    u32 get3DScisDiv2X(u8);
    u32 get3DScisDiv2Y(u8);
    u32 get3DScisDiv2W(u8);
    u32 get3DScisDiv2H(u8);
    u32 get3DScisDiv4X(u8);
    u32 get3DScisDiv4Y(u8);
    u32 get3DScisDiv4W(u8);
    u32 get3DScisDiv4H(u8);
    u32 get3DScisSubX();
    u32 get3DScisSubY();
    u32 get3DScisSubW();
    u32 get3DScisSubH();
    f32 getOrthoL();
    f32 getOrthoR();
    f32 getOrthoT();
    f32 getOrthoB();
    f32 getPerspAspect();
    f32 getPerspAspectDiv2();
    f32 getPerspAspectDiv4();
    f32 getPerspAspectSub();

    extern JFWDisplay *mspDisplay;
    extern JKRExpHeap *mspAppHeap;
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