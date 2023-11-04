#include <dolphin/os.h>
#include "kartLocale.h"
#include "JSystem/J3D/J3DSys.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "Inagaki/GameAudioMain.h"
#include "Kaneshige/Movie/MovieApp.h"
#include "Kaneshige/Movie/MoviePlayer.h"
#include "Kaneshige/ScrnShot.h"
#include "Kaneshige/SysDebug.h"
#include "Osako/AppMgr.h"
#include "Osako/BBAMgr.h"
#include "Osako/CardAgent.h"
#include "Osako/CardMgr.h"
#include "Osako/clock.h"
#include "Osako/ErrorViewApp.h"
#include "Osako/FontMgr.h"
#include "Osako/LogoApp.h"
#include "Osako/NetGameMgr.h"
#include "Osako/PadMgr.h"
#include "Osako/ResetAgent.h"
#include "Osako/ResMgr.h"
#include "Osako/SequenceApp.h"
#include "Osako/system.h"
#include "Osako/systemData.h"
#include "Osako/SystemRecord.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

// .rodata
#ifdef DEBUG
static const float lbl_8037d5e8[4] = {0.0f, 0.0f, 0.0f, 1.0f};
#pragma push
#pragma force_active on
DUMMY_POINTER(lbl_8037d5e8)
#pragma pop
#endif

#ifdef VIDEO_PAL
#define BOOT_RENDERMODE &SystemData::scPalInt448Df
#else
#define BOOT_RENDERMODE &SystemData::scNtscInt448Df
#endif

namespace System {
    JFWDisplay *mspDisplay;
    JKRExpHeap *mspAppHeap;
    int msDvdState;
    int msRenderMode;
    JKRTask *mspSendTask;
    JKRTask *mspRecvTask;
    JKRTask *mspAramTask;
    JKRTask *mspLoTask;
    J2DPrint *mspJ2DPrint;
    J2DOrthoGraph *mspJ2DOrtho;
    JKRSolidHeap *mspAudioHeap;

    void startAudioTask(void*) {
        void *audioFile = JKRDvdRipper::loadToMainRAM("AudioRes/GCKart.baa", nullptr, Switch_1, 
                            0, SequenceApp::mspSequenceApp->getHeap(), JKRDvdRipper::ALLOC_DIR_BOTTOM, 0, nullptr, nullptr);
        GetGameAudioMain()->init(mspAudioHeap, SystemData::scAudioAramSize, audioFile, 0, 0);
        delete audioFile;
        gSystemRecord.applyAudioSetting();
    }

    void init() {
#ifdef REGION_EU
        switch(OSGetLanguage()) {
        case OS_LANG_ENGLISH:
            KartLocale::setCountry(UNITED_KINGDOM);
            break;
        case OS_LANG_FRENCH:
            KartLocale::setCountry(FRANCE);
            break;
        case OS_LANG_GERMAN:
            KartLocale::setCountry(GERMANY);
            break;
        case OS_LANG_ITALIAN:
            KartLocale::setCountry(ITALY);
            break;
        case OS_LANG_SPANISH:
            KartLocale::setCountry(SPAIN);
            break;
        default: // fallback for when gamecube is set to Dutch?
            KartLocale::setCountry(UNITED_KINGDOM);
            break;
        }
#endif
        KartLocale::localize();

        JFWSystem::setMaxStdHeap(SystemData::scNumStandardHeaps);
        JFWSystem::setSysHeapSize(SystemData::scSystemHeapSize);
        JFWSystem::setFifoBufSize(SystemData::scDefaultFifoSize);
        JFWSystem::setAramAudioBufSize(SystemData::scAudioAramSize);
        JFWSystem::setAramGraphBufSize(0xffffffff);
        JFWSystem::setRenderMode(BOOT_RENDERMODE);
        msRenderMode = NORMAL_RMODE;

        JFWSystem::init();
        PadMgr::init();
        OSInitFastCast();

        JKRAramStream::setTransBuffer(nullptr, 0x8000, JKRGetSystemHeap());
        JUTException::setPreUserCallback(callbackException);
#ifdef DEBUG
        /* 
        don't make this a static name, since debugInfoS also exists
        however if the "Debug Support" build gets found then change this
        */
        JUTException::appendMapFile("debugInfoM.MAP"); 
#endif

        mspSendTask = JKRTask::create(4, 1, 0x4000, nullptr);
        mspRecvTask = JKRTask::create(4, 2, 0x4000, nullptr);
        mspAramTask = JKRTask::create(64, 14, 0x4000, nullptr);
        mspLoTask = JKRTask::create(64, 18, 0x4000, nullptr);
        mspAudioHeap = JKRCreateSolidHeap(SystemData::scAudioHeapSize, JKRGetRootHeap(), false);

        GetGameAudioMain()->bootDSP();
        mspLoTask->request(startAudioTask, nullptr, nullptr);

        SysDebug::createManager();

        SYSDBG_CreateHeapInfo(mspAudioHeap, "Audio");
        SYSDBG_CreateHeapInfo(JKRGetRootHeap(), "root");
        SYSDBG_CreateHeapInfo(JKRGetSystemHeap(), "JSystem");
        SYSDBG_SetHeapGroup("Resource", JKRGetRootHeap());
        ResMgr::create(JKRGetRootHeap());

        SYSDBG_SetHeapGroup("Font", JKRGetRootHeap());
        FontMgr::create(JKRGetRootHeap());

        mspJ2DOrtho = new J2DOrthoGraph(getOrthoL(), getOrthoT(), getOrthoR(), getOrthoB(), -1.0f, 1.0f);
        mspJ2DPrint = new J2DPrint(FontMgr::mspDebugFont, 0.0f); // propably a getter for debugfont

        SYSDBG_SetHeapGroup("Display", JKRGetRootHeap());
        mspDisplay = JFWDisplay::createManager(nullptr, JKRGetRootHeap(), JUTXfb::DoubleBuffer, true);

        mspDisplay->setFader(new JUTFader(SystemData::sc3DScissor.X, SystemData::sc3DScissor.Y, 
                            SystemData::sc3DScissor.W, SystemData::sc3DScissor.H, JUtility::TColor(0, 0, 0, 0xff)));

        SYSDBG_SetHeapGroup("Card", JKRGetRootHeap());
        CardMgr::create();
        CardAgent::create(JKRGetRootHeap());

        SYSDBG_SetHeapGroup("Net", JKRGetRootHeap());
        BBAMgr::create(JKRGetRootHeap());
        NetGameMgr::create(JKRGetRootHeap());
        JUTVideo::getManager()->setPostRetraceCallback(NetGameMgr::retraceCallback);

        SYSDBG_SetDefaultHeapGroup(JKRGetRootHeap());
        JKRGetRootHeap()->becomeCurrentHeap();
        SYSDBG_SetDefaultHeapGroup(JKRGetRootHeap());

        JUTProcBar::getManager()->setVisible(false);
        JUTProcBar::getManager()->setHeapBarVisible(false);

        mspAppHeap = JKRCreateExpHeap(0xffffffff, JKRGetRootHeap(), false);
        mspAppHeap->becomeCurrentHeap();
        SYSDBG_CreateHeapInfo(mspAppHeap, "App");

        ErrorViewApp::create();
        LogoApp::create();
        SequenceApp::create();
        MovieApp::create();
        reset();
        LogoApp::call();

        JUTGamePad::setResetCallback(ResetAgent::callbackReset, nullptr);
    }

    void reset()
    {
        mspDisplay->set_38(1);
        changeNormalRenderMode();
    }

    /*
    NTSC
    0 = normal render mode(NTSC 480i)
    1 = movie render mode (NTSC 480i)
    2 = normal render mode(progressive scan)
    3 = movie render mode (progressive scan)
    PAL
    4 = normal render mode(50hz)
    5 = movie render mode (50hz)
    6 = normal render mode(60hz)
    7 = movie render mode(60hz)
    */
#ifndef VIDEO_PAL // TODO: Other functions, unfortunately the define approach won't work well since PAL uses 3 cases(like changePAL50())
    void changeProgressive()
    {
        switch (msRenderMode)
        {
        case NORMAL_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scNtscProg448Soft);
            msRenderMode = ENHANCED_RMODE;
            break;
        case MOVIE_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scNtscProg448);
            msRenderMode = ENHANCED_MOVIE_RMODE;
            break;
        }
    }
#else
    void changeEuRgb60()
    {
        switch (msRenderMode)
        {
        case NORMAL_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scEuRgb60Int448Df);
            KartLocale::setVideoFrameMode(VideoFrameMode_60Hz);
            msRenderMode = ENHANCED_RMODE;
            break;
        case MOVIE_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scEuRgb60Int448);
            KartLocale::setVideoFrameMode(VideoFrameMode_60Hz);
            msRenderMode = ENHANCED_MOVIE_RMODE;
            break;
        }
    }

    void changePal50()
    {
        switch (msRenderMode)
        {
        case ENHANCED_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scPalInt448Df);
            KartLocale::setVideoFrameMode(VideoFrameMode_50Hz);
            msRenderMode = NORMAL_RMODE;
            break;
        case ENHANCED_MOVIE_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scPalInt448);
            KartLocale::setVideoFrameMode(VideoFrameMode_50Hz);
            msRenderMode = MOVIE_RMODE;
            break;

        }
    }
#endif

    // perhaps create defines for the rendermode tables
    void changeMovieRenderMode()
    {
        switch (msRenderMode) 
        {
#ifndef VIDEO_PAL
        case NORMAL_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scNtscInt448);
            msRenderMode = MOVIE_RMODE;
            break;
        case ENHANCED_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scNtscProg448);
            msRenderMode = ENHANCED_MOVIE_RMODE;
            break;
#else
        case NORMAL_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scPalInt448);
            msRenderMode = MOVIE_RMODE;
            break;
        case ENHANCED_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scEuRgb60Int448);
            msRenderMode = ENHANCED_MOVIE_RMODE;
#endif
        }

    }

    void changeNormalRenderMode()
    {
        switch (msRenderMode)
        {
#ifndef VIDEO_PAL
        case MOVIE_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scNtscInt448Df);
            msRenderMode = NORMAL_RMODE;
            break;
        case ENHANCED_MOVIE_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scNtscProg448Soft);
            msRenderMode = ENHANCED_RMODE;
            break;
#else
        case MOVIE_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scPalInt448Df);
            msRenderMode = NORMAL_RMODE;
            break;
        case ENHANCED_MOVIE_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scEuRgb60Int448Df);
            msRenderMode = ENHANCED_RMODE;
#endif
        }

    }

    void callbackException(OSError p1, OSContext * p2, u32 p3, u32 p4)
    {
        haltRumble();
        mspDisplay->startFadeIn(0);
#ifndef DEBUG
        JUTException::getManager()->setGamePad((JUTGamePad *)-1); // set to -1 to reset all controllers?
        JUTException::waitTime(2000);

        u16 exceptionInputs[] = {
            PAD_BUTTON_A, PAD_BUTTON_B, PAD_BUTTON_X, PAD_BUTTON_Y,
            PAD_TRIGGER_L, PAD_TRIGGER_R, PAD_TRIGGER_Z, 0x0
        };

        u32 pressedButton;
        for (int i = 0; exceptionInputs[i] != 0;) // keep looping on this until the right button sequence has been pressed
        {
            JUTException::waitTime(100);
            JUTException::getManager()->readPad(&pressedButton, nullptr);
            if (pressedButton != 0)
            {
                i = (pressedButton == exceptionInputs[i]) ? ++i : 0; // if current button doesn't match what's expected, reset i to 0 to restart the sequence
            }
        }

        JUTException::getManager()->setGamePad(nullptr);
#endif
    }

#ifdef DEBUG
    void haltRumble() {
        stopMotors();

        LGWheels *wheels = JUTGamePad::getLGWheels();

        for(s32 i = 0; i < 4; i++) {
            if (wheels->IsConnected(i)) {
                wheels->StopBumpyRoadEffect(i);
                wheels->StopSlipperyRoadEffect(i);
                wheels->StopCarAirborne(i);
            }
        }
    }
#endif

    void checkDVDState()
    {
        msDvdState = DVDGetDriveStatus();
        switch (msDvdState) {
        case DVD_STATE_COVER_OPEN:
            ErrorViewApp::call(ErrorViewApp::ERROR1);
            break;
        case DVD_STATE_NO_DISK:
            ErrorViewApp::call(ErrorViewApp::ERROR2);
            break;
        case DVD_STATE_WRONG_DISK:
            ErrorViewApp::call(ErrorViewApp::ERROR3);
            break;
        case DVD_STATE_RETRY:
            ErrorViewApp::call(ErrorViewApp::ERROR4);
            break;
        case DVD_STATE_FATAL_ERROR:
            ErrorViewApp::call(ErrorViewApp::ERROR5);
            break;
        }
    }

    void beginFrame() {
        CardMgr::probe();
        checkDVDState();
        if (GetGameAudioMain()->isActive())
            GetGameAudioMain()->frameWork();
        mspDisplay->beginRender();
        NetGameMgr::mspNetGameMgr->adjustFrame();
        Clock::move();
        j3dSys.drawInit();

        PadMgr::framework();
        NetGameMgr::mspNetGameMgr->framework();        
    }

    void endRender() {
#ifdef DEBUG
        SysDebug::getManager()->draw();
        if (ScrnShot::sScrnShot)
            ScrnShot::sScrnShot->capture();
#endif
        mspDisplay->endRender();
#ifdef DEBUG
        if (gGamePad1P.testButton(JUTGamePad::L) && gGamePad1P.testTrigger(JUTGamePad::DPAD_DOWN)) {
            GetGameAudioMain()->setBgmVolume(0.0f);
        }        
#endif
    }

    void endFrame() {
#ifdef DEBUG
        SysDebug::getManager()->ctrlDebugMode();
#endif
        mspDisplay->endFrame();
        if(MoviePlayer::sPlayer) {
            MoviePlayer::sPlayer->drawDone();
        }
    }

    void run() {
        // not sure if this is how it was originally written however the functions match in size
        while(true) { 
            beginFrame();
            AppMgr::draw();
            endRender();
            AppMgr::calc();
            endFrame();
        }
    }

    // screw you nintendo if these have another inline
    f32 get2DVpX() { return SystemData::sc3DViewPort.X; }
    f32 get2DVpY() { return SystemData::sc3DViewPort.Y; }
    f32 get2DVpW() { return SystemData::sc3DViewPort.W; }
    f32 get2DVpH() { return SystemData::sc3DViewPort.H; }

    f32 get3DVpX() { return SystemData::sc3DViewPort.X; }
    f32 get3DVpY() { return SystemData::sc3DViewPort.Y; }
    f32 get3DVpW() { return SystemData::sc3DViewPort.W; }
    f32 get3DVpH() { return SystemData::sc3DViewPort.H; }

    // dumb formatting
#line 788
    f32 get3DVpDiv2X(u8 pos) { JUT_ASSERT(pos < 2); return SystemData::sca3DViewPortDiv2[(u32)pos].X; }
    f32 get3DVpDiv2Y(u8 pos) { JUT_ASSERT(pos < 2); return SystemData::sca3DViewPortDiv2[(u32)pos].Y; }
    f32 get3DVpDiv2W(u8 pos) { JUT_ASSERT(pos < 2); return SystemData::sca3DViewPortDiv2[(u32)pos].W; }
    f32 get3DVpDiv2H(u8 pos) { JUT_ASSERT( pos < 2); return SystemData::sca3DViewPortDiv2[(u32)pos].H; }

    f32 get3DVpDiv4X(u8 pos) { JUT_ASSERT(pos < 4); return SystemData::sca3DViewPortDiv4[(u32)pos].X; }
    f32 get3DVpDiv4Y(u8 pos) { JUT_ASSERT(pos < 4); return SystemData::sca3DViewPortDiv4[(u32)pos].Y; }
    f32 get3DVpDiv4W(u8 pos) { JUT_ASSERT(pos < 4); return SystemData::sca3DViewPortDiv4[(u32)pos].W; }
    f32 get3DVpDiv4H(u8 pos) { JUT_ASSERT(pos < 4); return SystemData::sca3DViewPortDiv4[(u32)pos].H; }

    f32 get3DVpSubX() { return SystemData::sc3DViewPortSub.X; }
    f32 get3DVpSubY() { return SystemData::sc3DViewPortSub.Y; }
    f32 get3DVpSubW() { return SystemData::sc3DViewPortSub.W; }
    f32 get3DVpSubH() { return SystemData::sc3DViewPortSub.H; } // Unused

    // Scissor data
    u32 get2DScisX() { return SystemData::sc3DScissor.X; }
    u32 get2DScisY() { return SystemData::sc3DScissor.Y; }
    u32 get2DScisW() { return SystemData::sc3DScissor.W; }
    u32 get2DScisH() { return SystemData::sc3DScissor.H; }

    u32 get3DScisX() { return SystemData::sc3DScissor.X; }
    u32 get3DScisY() { return SystemData::sc3DScissor.Y; }
    u32 get3DScisW() { return SystemData::sc3DScissor.W; }
    u32 get3DScisH() { return SystemData::sc3DScissor.H; }

    u32 get3DAwardScisX() { return SystemData::sc3DAwardScissor.X; }
    u32 get3DAwardScisY() { return SystemData::sc3DAwardScissor.Y; }
    u32 get3DAwardScisW() { return SystemData::sc3DAwardScissor.W; }
    u32 get3DAwardScisH() { return SystemData::sc3DAwardScissor.H; }

    u32 get3DScisDiv2X(u8 pos) { JUT_ASSERT(pos < 2); return SystemData::sca3DScissorDiv2[(u32)pos].X; }
    u32 get3DScisDiv2Y(u8 pos) { JUT_ASSERT(pos < 2); return SystemData::sca3DScissorDiv2[(u32)pos].Y; }
    u32 get3DScisDiv2W(u8 pos) { JUT_ASSERT(pos < 2); return SystemData::sca3DScissorDiv2[(u32)pos].W; }
    u32 get3DScisDiv2H(u8 pos) { JUT_ASSERT(pos < 2); return SystemData::sca3DScissorDiv2[(u32)pos].H; }

    u32 get3DScisDiv4X(u8 pos) { JUT_ASSERT(pos < 4); return SystemData::sca3DScissorDiv4[(u32)pos].X; }
    u32 get3DScisDiv4Y(u8 pos) { JUT_ASSERT(pos < 4); return SystemData::sca3DScissorDiv4[(u32)pos].Y; }
    u32 get3DScisDiv4W(u8 pos) { JUT_ASSERT(pos < 4); return SystemData::sca3DScissorDiv4[(u32)pos].W; }
    u32 get3DScisDiv4H(u8 pos) { JUT_ASSERT(pos < 4); return SystemData::sca3DScissorDiv4[(u32)pos].H; }

    u32 get3DScisSubX() { return SystemData::sc3DScissorSub.X; }
    u32 get3DScisSubY() { return SystemData::sc3DScissorSub.Y; }
    u32 get3DScisSubW() { return SystemData::sc3DScissorSub.W; }
    u32 get3DScisSubH() { return SystemData::sc3DScissorSub.H; }
    // different struct, perhaps TBox2?
    f32 getOrthoL() { return SystemData::scOrtho.X; }
    f32 getOrthoR() { return SystemData::scOrtho.W; }
    f32 getOrthoT() { return SystemData::scOrtho.Y; }
    f32 getOrthoB() { return SystemData::scOrtho.H; }

    f32 getPerspAspect() { return SystemData::scAspect; }
    f32 getPerspAspectDiv2() { return SystemData::scAspectDiv2; }
    f32 getPerspAspectDiv4() { return SystemData::scAspect; }
    f32 getPerspAspectSub() { return SystemData::scAspectSub; }
}
