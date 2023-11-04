#ifndef SYSTEM_H
#define SYSTEM_H

#include <dolphin/os.h>
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPrint.h"
#include "JSystem/JUtility/JUTException.h"
#include "JSystem/JFramework/JFWDisplay.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRAssertHeap.h"
#include "JSystem/JKernel/JKRThread.h"
#include "types.h"

namespace System
{    
    void init();
    void reset();
    void changeProgressive();
    void changePal50();
    void changeEuRgb60();
    void changeMovieRenderMode();
    void changeNormalRenderMode();
    inline void stopMotors() {
        PADControlMotor(0, 0);
        PADControlMotor(1, 0);
        PADControlMotor(2, 0);
        PADControlMotor(3, 0);
    }

    void haltRumble();
#ifndef DEBUG
    inline void haltRumble()
    {
        stopMotors();
    }
#endif

    void run();
    // Unused functions however i'm sure these get auto inlined in run()
    void checkDVDState();
    void beginFrame();
    void endRender();
    void endFrame();

    // callbacks
    void startAudioTask(void*);
    void callbackException(OSError error, OSContext *context, u32 p3, u32 p4);

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
    inline JKRTask *getLoTask() { return mspLoTask; }
    inline JKRTask *getAramTask() { return mspAramTask; }
};


#ifndef REGION_EU
    #define SYSTEM_ChangeVideoMode() \
        System::changeProgressive()
#else
#define SYSTEM_ChangeVideoMode() \
    System::changeEuRgb60()
#endif

#define SYSTEM_GetFader() \
    System::getDisplay()->getFader()
#define SYSTEM_GetFaderStatus() \
    System::getDisplay()->getFader()->getStatus()
#define SYSTEM_StartFadeIn(duration) \
    System::getDisplay()->startFadeIn(duration)
#define SYSTEM_StartFadeOut(duration) \
    System::getDisplay()->startFadeOut(duration)

#define SYSTEM_RequestTask(function, arg, msg) \
    System::getLoTask()->request(function, arg, msg)

#define SYSTEM_IsFadingIn() \
    SYSTEM_GetFaderStatus() == JUTFader::Status_In
#define SYSTEM_IsFadingOut() \
    SYSTEM_GetFaderStatus() == JUTFader::Status_Out

#define SYSTEM_GetAppHeap() \
    System::getAppHeap()

#endif
