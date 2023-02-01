#include "kartlocale.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "JSystem/JFrameWork/JFWSystem.h"
#include "JSystem/JKernel/JKRDvdRipper.h"
#include "Inagaki/GameAudioMain.h"
#include "Kaneshige/SysDebug.h"
#include "Osako/ResMgr.h"
#include "Osako/SequenceApp.h"
#include "Osako/system.h"
#include "Osako/systemData.h"
#include "Osako/SystemRecord.h"

// .rodata
static const float lbl_8037d5e8[4] = {0.0f, 0.0f, 0.0f, 1.0f};
#pragma push
#pragma force_active on
DUMMY_POINTER(lbl_8037d5e8)
#pragma pop
#include "orderstrings/8037d60c_8037d670.inc"

namespace System {
    void startAudioTask(void* p1) {
        void *audioFile = JKRDvdRipper::loadToMainRAM("AudioRes/GCKart.baa", nullptr, Switch_1, 0, SequenceApp::mspSequenceApp->getHeap(), JKRDvdRipper::ALLOC_DIR_BOTTOM, 0, nullptr, nullptr);
        GameAudio::Main::getAudio()->init(mspAudioHeap, SystemData::scAudioAramSize, audioFile, 0, 0);
        delete audioFile;
        gSystemRecord.applyAudioSetting();
    }

    /*void init() {
        KartLocale::localize();

        JFWSystem::setMaxStdHeap(SystemData::scNumStandardHeaps);
        JFWSystem::setSysHeapSize(SystemData::scSystemHeapSize);
        JFWSystem::setFifoBufSize(SystemData::scDefaultFifoSize);
        JFWSystem::setAramAudioBufSize(SystemData::scAudioAramSize);
        JFWSystem::setAramGraphBufSize(0xffffffff);
        JFWSystem::setRenderMode(&SystemData::scNtscInt448Df);
        msRenderMode = 0;

        JFWSystem::init();
        PadMgr::init();

        JKRAramStream::setTransBuffer(nullptr, 0x8000, JKRGetSystemHeap());
        JUTException::setPreUserCallback(callbackException);
        JUTException::appendMapFile("debugInfoM.MAP");

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

        mspDisplay->setFader(new JUTFader(SystemData::sc3DScissor.H, SystemData::sc3DScissor.W, SystemData::sc3DScissor.X, SystemData::sc3DScissor.Y, 0xff)); // not sure if this is the right way to do it but we'll see

        SysDebug::getManager()->setHeapGroup("Card", JKRGetRootHeap());
        CardMgr::create();
        CardAgent::create();

        SysDebug::getManager()->setHeapGroup("Net", JKRGetRootHeap());
        BBAMgr::create();
        NetGameMgr::create();
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

        JUTGamePad::C3ButtonReset::sCallback = ResetAgent::callbackReset;
        JUTGamePad::C3ButtonReset::sCallbackArg = 0;
    }*/
}

// .sdata2
#include "orderstrings/8041b688_8041b6b4.inc"
#include "orderfloatsm/8041b6b4_8041b6c0.inc"
#include "orderstrings/8041b6c0_8041b6e8.inc"

asm void System::init()
{
#include "asm/801feba0.s"
}


namespace System {
    void reset()
    {
        mspDisplay->_38 = 1;
        changeNormalRenderMode();
    }

    // change rendermode stuff is just a guess, so probably won't match
    /*
    0 = normal render mode(interlaced)
    1 = movie render mode (interlaced)
    2 = normal render mode(progressive scan)
    3 = movie render mode (progressive scan)
    these are for PAL? (just guessing), could be other way around
    4 = normal render mode(50hz) 
    5 = movie render mode (60hz)
    */
    void changeProgressive()
    {
        switch (msRenderMode)
        case 0:
        {
            JUTVideo::getManager()->setRenderMode(SystemData::scNtscProg448Soft);
            msRenderMode = 2;
        }
        case 1:
        {
            JUTVideo::getManager()->setRenderMode(SystemData::scNtscProg448);
            msRenderMode = 3;
            break;
        }
    }

    void changeMovieRenderMode()
    {
        switch (msRenderMode)
        case 0:
        {
            JUTVideo::getManager()->setRenderMode(SystemData::scNtscInt448);
            msRenderMode = 1;
        }
        case 2:
        {
            JUTVideo::getManager()->setRenderMode(SystemData::scNtscProg448);
            msRenderMode = 3;
            break;
        }
    }

    void changeNormalRenderMode()
    {
        switch (msRenderMode)
        case 1:
        {
            JUTVideo::getManager()->setRenderMode(SystemData::scNtscInt448Df);
            msRenderMode = 0;
        }
        case 3:
        {
            JUTVideo::getManager()->setRenderMode(SystemData::scNtscProg448Soft);
            msRenderMode = 2;
            break;
        }
    }

    void callbackException(u16, OSContext *, u32, u32)
    {
        haltRumble();
        JUTFader *fader = mspDisplay->mFader; // getFader() or inline startfadeIn
        if (fader)
            fader->startFadeIn(0);
    }

    /*void haltRumble() {
        PADControlMotor(0, 0);
        PADControlMotor(2, 0);
        PADControlMotor(3, 0);
        PADControlMotor(4, 0);

        JUTGamePad::LGWheels wheels = JUTGamePad::getLGWheels();

        for(int i = 0; i < 4; i++) {
            if (wheels->IsConnected(i)) {
                wheels->StopBumpyRoadEffect(i);
                wheels->StopSlipperyRoadEffect(i);
                wheels->StopCarAirborne(i);
            }
        }
    }*/

    /*void checkDVDState()
    {
        msDvdState = DVDGetDriveStatus();
        switch (msDvdState) {
        case 5:
            ErrorViewApp::call(1);
            break;
        case 4:
            ErrorViewApp::call(2);
            break;
        case 6:
            ErrorViewApp::call(3);
            break;
        case 11:
            ErrorViewApp::call(4);
            break;
        case -1:
            ErrorViewApp::call(5);
            break;
        }
    }*/

    /*void endRender() {
        mspDisplay->endRender();
        if (gGamePad1P.testButton(JUTGamePad::L) && gGamePad1P.testTrigger(JUTGamePad::DPAD_DOWN)) {
            GameAudio::Main::getAudio()->setBgmVolume(0.0f);
        }
        AppMgr::calc();
        SysDebug::getManager()->ctrlDebugMode();
    }*/

    /*void endFrame() {
        mspDisplay->endFrame();
        if(MoviePlayer::sPlayer) {
            MoviePlayer::sPlayer->drawDone();
        }
    }*/
}


asm void System::haltRumble()
{
#include "asm/801ff294.s"
}

asm void System::run()
{
#include "asm/801ff334.s"
}

namespace System {
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
    return SystemData::sca3DScissorDiv2[pos].X;
}

f32 get3DVpDiv2Y(u8 pos) {
    JUT_ASSERT(789, pos < 2);
    return SystemData::sca3DScissorDiv2[pos].Y;
}

f32 get3DVpDiv2W(u8 pos) {
    JUT_ASSERT(790, pos < 2);
    return SystemData::sca3DScissorDiv2[pos].W;
}

f32 get3DVpDiv2H(u8 pos) {
    JUT_ASSERT(790, pos < 2);
    return SystemData::sca3DScissorDiv2[pos].H;
}

f32 get3DVpDiv4X(u8 pos) {
    JUT_ASSERT(793, pos < 4);
    return SystemData::sca3DViewPortDiv4[pos].X;
}

f32 get3DVpDiv4Y(u8 pos) {
    JUT_ASSERT(794, pos < 4);
    return SystemData::sca3DViewPortDiv4[pos].Y;
}

f32 get3DVpDiv4W(u8 pos) {
    JUT_ASSERT(795, pos < 4);
    return SystemData::sca3DViewPortDiv4[pos].W;
}

f32 get3DVpDiv4H(u8 pos) {
    JUT_ASSERT(796, pos < 4);
    return SystemData::sca3DViewPortDiv4[pos].H;
}

f32 get3DVpSubX() { return SystemData::sc3DViewPortSub.X; }
f32 get3DVpSubY() { return SystemData::sc3DViewPortSub.Y; }
f32 get3DVpSubW() { return SystemData::sc3DViewPortSub.W; }
f32 get3DVpSubH() { return SystemData::sc3DViewPortSub.H; } // Unused, perhaps inline

f32 get2DScisX() { return SystemData::sc3DScissor.X; }
f32 get2DScisY() { return SystemData::sc3DScissor.Y; }
f32 get2DScisW() { return SystemData::sc3DScissor.W; }
f32 get2DScisH() { return SystemData::sc3DScissor.H; }

f32 get3DScisX() { return SystemData::sc3DScissor.X; }
f32 get3DScisY() { return SystemData::sc3DScissor.Y; }
f32 get3DScisW() { return SystemData::sc3DScissor.W; }
f32 get3DScisH() { return SystemData::sc3DScissor.H; }

f32 get3DAwardScisX() { return SystemData::sc3DAwardScissor.X; }
f32 get3DAwardScisY() { return SystemData::sc3DAwardScissor.Y; }
f32 get3DAwardScisW() { return SystemData::sc3DAwardScissor.W; }
f32 get3DAwardScisH() { return SystemData::sc3DAwardScissor.H; }

f32 get3DScisDiv2X(u8 pos) {
    JUT_ASSERT(819, pos < 2);
    return SystemData::sca3DScissorDiv2[pos].X;
}

f32 get3DScisDiv2Y(u8 pos) {
    JUT_ASSERT(820, pos < 2);
    return SystemData::sca3DScissorDiv2[pos].Y;
}

f32 get3DScisDiv2W(u8 pos) {
    JUT_ASSERT(821, pos < 2);
    return SystemData::sca3DScissorDiv2[pos].W;
}

f32 get3DScisDiv2H(u8 pos) {
    JUT_ASSERT(822, pos < 2);
    return SystemData::sca3DScissorDiv2[pos].H;
}

f32 get3DScisDiv4X(u8 pos) {
    JUT_ASSERT(824, pos < 2);
    return SystemData::sca3DScissorDiv2[pos].X;
}

f32 get3DScisDiv4Y(u8 pos)
{
    JUT_ASSERT(825, pos < 4);
    return SystemData::sca3DScissorDiv2[pos].Y;
}

f32 get3DScisDiv4W(u8 pos)
{
    JUT_ASSERT(826, pos < 4);
    return SystemData::sca3DScissorDiv2[pos].W;
}

f32 get3DScisDiv4H(u8 pos)
{
    JUT_ASSERT(827, pos < 4);
    return SystemData::sca3DScissorDiv2[pos].W;
}

f32 get3DScisSubX() { return SystemData::sc3DScissorSub.X; }
f32 get3DScisSubY() { return SystemData::sc3DScissorSub.Y; }
f32 get3DScisSubW() { return SystemData::sc3DScissorSub.W; }
f32 get3DScisSubH() { return SystemData::sc3DScissorSub.H; }
// perhaps create new struct
f32 getOrthoL() { return SystemData::scOrtho.X; }
f32 getOrthoR() { return SystemData::scOrtho.Y; }
f32 getOrthoT() { return SystemData::scOrtho.W; }
f32 getOrthoB() { return SystemData::scOrtho.H; }

f32 getPerspAspect() {return SystemData::scAspect; }
f32 getPerspAspectDiv2() { return SystemData::scAspectDiv2; }
f32 getPerspAspectDiv4() { return SystemData::scAspect; }
f32 getPerspAspectSub() { return SystemData::scAspectSub; }
}