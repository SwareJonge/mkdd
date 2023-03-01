#include <dolphin/os.h>
#include "kartLocale.h"
#include "JSystem/J3D/J3DSys.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "JSystem/JUtility/JUTGamePad.h"
#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "Inagaki/GameAudioMain.h"
#include "Kaneshige/MovieApp.h"
#include "Kaneshige/MoviePlayer.h"
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

// .rodata
#if EU_RELEASE
#define NORMAL_RMODE 4         // PAL 50hz
#define MOVIE_RMODE 5          // PAL 50hz
#define ENHANCED_RMODE 6       // EURGB 60hz
#define ENHANCED_MOVIE_RMODE 7 // EURGB 60hz
// for whatever reason PAL doesn't seem to have the garbage floats
#else
#define NORMAL_RMODE 0         // NTSC 480i
#define MOVIE_RMODE 1          // NTSC 480i
#define ENHANCED_RMODE 2       // 480p
#define ENHANCED_MOVIE_RMODE 3 // 480p
static const float lbl_8037d5e8[4] = {0.0f, 0.0f, 0.0f, 1.0f};
#pragma push
#pragma force_active on
DUMMY_POINTER(lbl_8037d5e8)
#pragma pop
#endif

namespace System {
    JFWDisplay *System::mspDisplay;
    JKRExpHeap *System::mspAppHeap;
    int System::msDvdState;
    int System::msRenderMode;
    JKRTask *System::mspSendTask;
    JKRTask *System::mspRecvTask;
    JKRTask *System::mspAramTask;
    JKRTask *System::mspLoTask;
    J2DPrint *System::mspJ2DPrint;
    J2DOrthoGraph *System::mspJ2DOrtho;
    JKRSolidHeap *System::mspAudioHeap;

    void startAudioTask(void*) {
        void *audioFile = JKRDvdRipper::loadToMainRAM("AudioRes/GCKart.baa", nullptr, Switch_1, 
                            0, SequenceApp::mspSequenceApp->getHeap(), JKRDvdRipper::ALLOC_DIR_BOTTOM, 0, nullptr, nullptr);
        GameAudio::Main::getAudio()->init(mspAudioHeap, SystemData::scAudioAramSize, audioFile, 0, 0);
        delete audioFile;
        gSystemRecord.applyAudioSetting();
    }

    void init() {
        KartLocale::localize();

        JFWSystem::setMaxStdHeap(SystemData::scNumStandardHeaps);
        JFWSystem::setSysHeapSize(SystemData::scSystemHeapSize);
        JFWSystem::setFifoBufSize(SystemData::scDefaultFifoSize);
        JFWSystem::setAramAudioBufSize(SystemData::scAudioAramSize);
        JFWSystem::setAramGraphBufSize(0xffffffff);
        JFWSystem::setRenderMode(&SystemData::scNtscInt448Df);
        msRenderMode = NORMAL_RMODE;

        JFWSystem::init();
        PadMgr::init();
        OSInitFastCast();

        JKRAramStream::setTransBuffer(nullptr, 0x8000, JKRGetSystemHeap());
        JUTException::setPreUserCallback(callbackException);
#if DEBUG
        JUTException::appendMapFile("debugInfoM.MAP");
#endif

        mspSendTask = JKRTask::create(4, 1, 0x4000, nullptr);
        mspRecvTask = JKRTask::create(4, 2, 0x4000, nullptr);
        mspAramTask = JKRTask::create(64, 14, 0x4000, nullptr);
        mspLoTask = JKRTask::create(64, 18, 0x4000, nullptr);
        mspAudioHeap = JKRCreateSolidHeap(SystemData::scAudioHeapSize, JKRGetRootHeap(), false);

        GameAudio::Main::getAudio()->bootDSP();
        mspLoTask->request(startAudioTask, nullptr, nullptr);

        SysDebug::createManager();

        SysDebug::getManager()->createHeapInfo(mspAudioHeap, "Audio");
        SysDebug::getManager()->createHeapInfo(JKRGetRootHeap(), "root");
        SysDebug::getManager()->createHeapInfo(JKRGetSystemHeap(), "JSystem");
        SysDebug::getManager()->setHeapGroup("Resource", JKRGetRootHeap());
        ResMgr::create(JKRGetRootHeap());

        SysDebug::getManager()->setHeapGroup("Font", JKRGetRootHeap());
        FontMgr::create(JKRGetRootHeap());

        mspJ2DOrtho = new J2DOrthoGraph(getOrthoL(), getOrthoT(), getOrthoR(), getOrthoB(), -1.0f, 1.0f);
        mspJ2DPrint = new J2DPrint(FontMgr::mspDebugFont, 0.0f); // propably a getter for debugfont

        SysDebug::getManager()->setHeapGroup("Display", JKRGetRootHeap());
        mspDisplay = JFWDisplay::createManager(nullptr, JKRGetRootHeap(), JUTXfb::UNK_2, true);

        mspDisplay->setFader(new JUTFader(SystemData::sc3DScissor.X, SystemData::sc3DScissor.Y, 
                            SystemData::sc3DScissor.W, SystemData::sc3DScissor.H, JUtility::TColor(0, 0, 0, 0xff)));

        SysDebug::getManager()->setHeapGroup("Card", JKRGetRootHeap());
        CardMgr::create();
        CardAgent::create(JKRGetRootHeap());

        SysDebug::getManager()->setHeapGroup("Net", JKRGetRootHeap());
        BBAMgr::create(JKRGetRootHeap());
        NetGameMgr::create(JKRGetRootHeap());
        JUTVideo::getManager()->setPostRetraceCallback(NetGameMgr::retraceCallback);

        SysDebug::getManager()->setDefaultHeapGroup(JKRGetRootHeap());
        JKRGetRootHeap()->becomeCurrentHeap();
        SysDebug::getManager()->setDefaultHeapGroup(JKRGetRootHeap());

        JUTProcBar::getManager()->setVisible(false);
        JUTProcBar::getManager()->setHeapBarVisible(false);

        mspAppHeap = JKRCreateExpHeap(0xffffffff, JKRGetRootHeap(), false);
        mspAppHeap->becomeCurrentHeap();
        SysDebug::getManager()->createHeapInfo(mspAppHeap, "App");

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
    0 = normal render mode(NTSC 480i)
    1 = movie render mode (NTSC 480i)
    2 = normal render mode(progressive scan)
    3 = movie render mode (progressive scan)
    these are for PAL? (just guessing), could be other way around
    4 = normal render mode(50hz)
    5 = movie render mode (50hz)
    6 = normal render mode(60hz)
    7 = movie render mode(60hz)
    */
#if EU_RELEASE // TODO: Other functions, unfortunately the define approach won't work well since PAL uses 3 cases(like changePAL50())
    void changePal50() {
        switch (msRenderMode)
        {
        case ENHANCED_MOVIE_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scPalInt448);
            KartLocale::msVideoFrameMode = KartLocale::PAL50; // use setter?
            msRenderMode = MOVIE_RMODE;
            break;
        case NORMAL_RMODE:
        case ENHANCED_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scPalInt448Df);
            KartLocale::msVideoFrameMode = KartLocale::PAL50;
            msRenderMode = NORMAL_RMODE;
            break;
        }
    }
#else
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
#endif

    void changeMovieRenderMode()
    {
        switch (msRenderMode) 
        {
        case NORMAL_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scNtscInt448);
            msRenderMode = MOVIE_RMODE;
            break;
        case ENHANCED_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scNtscProg448);
            msRenderMode = ENHANCED_MOVIE_RMODE;
            break;
        }
    }

    void changeNormalRenderMode()
    {
        switch (msRenderMode)
        {
        case MOVIE_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scNtscInt448Df);
            msRenderMode = NORMAL_RMODE;
            break;
        case ENHANCED_MOVIE_RMODE:
            JUTVideo::getManager()->setRenderMode(&SystemData::scNtscProg448Soft);
            msRenderMode = ENHANCED_RMODE;
            break;
        }
    }

    void callbackException(OSError p1, OSContext * p2, u32 p3, u32 p4)
    {
        haltRumble();
        mspDisplay->startFadeIn(0);
    }

    void haltRumble() {
        PADControlMotor(0, 0);
        PADControlMotor(1, 0);
        PADControlMotor(2, 0);
        PADControlMotor(3, 0);

        LGWheels *wheels = JUTGamePad::getLGWheels();

        for(s32 i = 0; i < 4; i++) {
            if (wheels->IsConnected(i)) {
                wheels->StopBumpyRoadEffect(i);
                wheels->StopSlipperyRoadEffect(i);
                wheels->StopCarAirborne(i);
            }
        }
    }

    void checkDVDState()
    {
        msDvdState = DVDGetDriveStatus();
        switch (msDvdState) {
        case 5:
            ErrorViewApp::call(ErrorViewApp::ERROR1);
            break;
        case 4:
            ErrorViewApp::call(ErrorViewApp::ERROR2);
            break;
        case 6:
            ErrorViewApp::call(ErrorViewApp::ERROR3);
            break;
        case 11:
            ErrorViewApp::call(ErrorViewApp::ERROR4);
            break;
        case -1:
            ErrorViewApp::call(ErrorViewApp::ERROR5);
            break;
        }
    }

    void beginFrame() {
        CardMgr::probe();
        checkDVDState();
        if (GameAudio::Main::getAudio()->isActive())
            GameAudio::Main::getAudio()->frameWork();
        mspDisplay->beginRender();
        NetGameMgr::mspNetGameMgr->adjustFrame();
        Clock::move();
        j3dSys.drawInit();

        PadMgr::framework();
        NetGameMgr::mspNetGameMgr->framework();        
    }

    void endRender() {
#if DEBUG
        SysDebug::getManager()->draw();
        if (ScrnShot::sScrnShot)
            ScrnShot::sScrnShot->capture();
#endif
        mspDisplay->endRender();
#if DEBUG
        if (gGamePad1P.testButton(JUTGamePad::L) && gGamePad1P.testTrigger(JUTGamePad::DPAD_DOWN)) {
            GameAudio::Main::getAudio()->setBgmVolume(0.0f);
        }        
#endif
    }

    void endFrame() {
#if DEBUG
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

    // how the hell did they format this
    f32 get3DVpDiv2X(u8 pos) {
        JUT_ASSERT(788, pos < 2);
        return SystemData::sca3DViewPortDiv2[(u32)pos].X;
    }
    f32 get3DVpDiv2Y(u8 pos) {
        JUT_ASSERT(789, pos < 2);
        return SystemData::sca3DViewPortDiv2[(u32)pos].Y;
    }
    f32 get3DVpDiv2W(u8 pos) {
        JUT_ASSERT(790, pos < 2);
        return SystemData::sca3DViewPortDiv2[(u32)pos].W;
    }
    f32 get3DVpDiv2H(u8 pos) {
        JUT_ASSERT(791, pos < 2);
        return SystemData::sca3DViewPortDiv2[(u32)pos].H;
    }

    f32 get3DVpDiv4X(u8 pos) {
        JUT_ASSERT(793, pos < 4);
        return SystemData::sca3DViewPortDiv4[(u32)pos].X;
    }
    f32 get3DVpDiv4Y(u8 pos) {
        JUT_ASSERT(794, pos < 4);
        return SystemData::sca3DViewPortDiv4[(u32)pos].Y;
    }
    f32 get3DVpDiv4W(u8 pos) {
        JUT_ASSERT(795, pos < 4);
        return SystemData::sca3DViewPortDiv4[(u32)pos].W;
    }
    f32 get3DVpDiv4H(u8 pos) {
        JUT_ASSERT(796, pos < 4);
        return SystemData::sca3DViewPortDiv4[(u32)pos].H;
    }

    f32 get3DVpSubX() { return SystemData::sc3DViewPortSub.X; }
    f32 get3DVpSubY() { return SystemData::sc3DViewPortSub.Y; }
    f32 get3DVpSubW() { return SystemData::sc3DViewPortSub.W; }
    f32 get3DVpSubH() { return SystemData::sc3DViewPortSub.H; } // Unused

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

    u32 get3DScisDiv2X(u8 pos) {
        JUT_ASSERT(819, pos < 2);
        return SystemData::sca3DScissorDiv2[(u32)pos].X;
    }
    u32 get3DScisDiv2Y(u8 pos) {
        JUT_ASSERT(820, pos < 2);
        return SystemData::sca3DScissorDiv2[(u32)pos].Y;
    }
    u32 get3DScisDiv2W(u8 pos) {
        JUT_ASSERT(821, pos < 2);
        return SystemData::sca3DScissorDiv2[(u32)pos].W;
    }
    u32 get3DScisDiv2H(u8 pos) {
        JUT_ASSERT(822, pos < 2);
        return SystemData::sca3DScissorDiv2[(u32)pos].H;
    }

    u32 get3DScisDiv4X(u8 pos) {
        JUT_ASSERT(824, pos < 4);
        return SystemData::sca3DScissorDiv4[(u32)pos].X;
    }
    u32 get3DScisDiv4Y(u8 pos) {
        JUT_ASSERT(825, pos < 4);
        return SystemData::sca3DScissorDiv4[(u32)pos].Y;
    }
    u32 get3DScisDiv4W(u8 pos) {
        JUT_ASSERT(826, pos < 4);
        return SystemData::sca3DScissorDiv4[(u32)pos].W;
    }
    u32 get3DScisDiv4H(u8 pos) {
        JUT_ASSERT(827, pos < 4);
        return SystemData::sca3DScissorDiv4[(u32)pos].H;
    }

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
