#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JUtility/JUTProcBar.h"
#include "JSystem/JFrameWork/JFWDisplay.h"

// Not tested yet, will add ecerything from TP, and edit whatever is needed
// https://github.com/zeldaret/tp/blob/master/libs/JSystem/JFramework/JFWDisplay.cpp

JSUList<OSAlarm> JFWAlarm::sList;

JFWDisplay *JFWDisplay::sManager;
static GXTexObj clear_z_tobj;

static Mtx44 e_mtx = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}};
static GXTexObj clear_z_TX[2] = {{0xFF00FF, 0xFF00FF, 0xFF00FF, 0xFF00FF, 0, 0xFF, 0, 0xFF, 0xFF00FF, 0xFF00FF, 0xFF, 0, -1},
                                 {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF, -1, -1}};

void JFWDisplay::ctor_subroutine(bool p1) {
    // UNUSED FUNCTION
    _24 = p1;
    _26 = 3;
    _08 = TCOLOR_BLACK;
    _0C = 0xFFFFFF;
    _14 = 0;
    mFader = nullptr;
    m_secondsPer60Frames = 1;
    _20 = 0;
    _28 = 0.0f;
    _30 = 0;
    _2C = OSGetTick();
    _34 = 0;
    _38 = 0;
    _3A = 0;
    mDrawDoneMethod = UNK_METHOD_0;
    clearEfb_init();
    JUTProcBar::create();
    JUTProcBar::clear();
}

JFWDisplay::JFWDisplay(JKRHeap *heap, JUTXfb::EXfbNumber bufferCount, bool p3) {
    // UNUSED FUNCTION
    ctor_subroutine(p3);
    mXfb = JUTXfb::createManager(heap, bufferCount);
}

JFWDisplay::~JFWDisplay() {
    if(JUTVideo::getManager() != nullptr) {
        waitBlanking(2);
    }
    JUTProcBar::destroy();
    JUTXfb::destroyManager();
    mXfb = nullptr;
}

JFWDisplay *JFWDisplay::createManager(const _GXRenderModeObj *renderModeObj, JKRHeap *heap, JUTXfb::EXfbNumber bufferCount, bool p4) {
    if (renderModeObj != nullptr)
        JUTVideo::sManager->setRenderMode(renderModeObj);

    if (sManager == nullptr)
        sManager = new JFWDisplay(heap, bufferCount, p4);

    return sManager;
}

void JFWDisplay::destroyManager() {
    delete sManager;
    sManager = nullptr;
}

static void callDirectDraw() {
    JUTChangeFrameBuffer(JUTXfb::getManager()->getDrawingXfb(),
                         JUTVideo::getManager()->getEfbHeight(),
                         JUTVideo::getManager()->getFbWidth());
    JUTAssertion::flushMessage();
}

void JFWDisplay::prepareCopyDisp() {
    u16 width = JUTVideo::getManager()->getFbWidth();
    u16 height = JUTVideo::getManager()->getEfbHeight();
    f32 y_scaleF = GXGetYScaleFactor(height, JUTVideo::getManager()->getXfbHeight());
    u16 line_num = GXGetNumXfbLines(height, y_scaleF);

    GXSetCopyClear(mClearColor, mZClear);
    GXSetDispCopySrc(0, 0, width, height);
    GXSetDispCopyDst(width, line_num);
    GXSetDispCopyYScale(y_scaleF);
    VIFlush();
    GXSetCopyFilter((GXBool)JUTVideo::getManager()->isAntiAliasing(),
                    JUTVideo::getManager()->getSamplePattern(), GX_ENABLE,
                    JUTVideo::getManager()->getVFilter());
    GXSetCopyClamp((GXFBClamp)mClamp);
    GXSetDispCopyGamma((GXGamma)mGamma);
    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    if (mEnableAlpha) {
        GXSetAlphaUpdate(GX_ENABLE);
    }
}

void JFWDisplay::drawendXfb_single() {
    JUTXfb *manager = JUTXfb::getManager();
    if (manager->getDrawingXfbIndex() >= 0) {
        prepareCopyDisp();
        JFWDrawDoneAlarm();
        GXFlush();
        manager->setDrawnXfbIndex(manager->getDrawingXfbIndex());
    }
}

void JFWDisplay::exchangeXfb_double() {
    JUTXfb *xfbMng = JUTXfb::getManager();

    if (xfbMng->getDrawnXfbIndex() == xfbMng->getDisplayingXfbIndex()) {
        if (xfbMng->getDrawingXfbIndex() >= 0)
        {
            if (_44 != nullptr)  {
                _44();
            }

            prepareCopyDisp();
            GXCopyDisp(xfbMng->getDrawingXfb(), GX_TRUE);
            if (mDrawDoneMethod == UNK_METHOD_0) {
                xfbMng->setDrawnXfbIndex(xfbMng->getDrawingXfbIndex());
                GXDrawDone();
                JUTVideo::dummyNoDrawWait();
            }
            else {
                JUTVideo::drawDoneStart();
            }

            if (mDrawDoneMethod == UNK_METHOD_0) {
                callDirectDraw();
            }
        }
        s16 cur_xfb_index = xfbMng->getDrawingXfbIndex();
        xfbMng->setDrawnXfbIndex(cur_xfb_index);
        xfbMng->setDrawingXfbIndex(cur_xfb_index >= 0 ? cur_xfb_index ^ 1 : 0);
    }
    else {
        clearEfb(mClearColor);
        if (xfbMng->getDrawingXfbIndex() < 0) {
            xfbMng->setDrawingXfbIndex(0);
        }
    }
}

void JFWDisplay::exchangeXfb_triple() {
    JUTXfb *xfbMng = JUTXfb::getManager();

    if (xfbMng->getDrawingXfbIndex() >= 0) {
        callDirectDraw();
    }

    xfbMng->setDrawnXfbIndex(xfbMng->getDrawingXfbIndex());

    s16 drawing_idx = xfbMng->getDrawingXfbIndex() + 1;
    do {
        if (drawing_idx >= 3 || drawing_idx < 0) {
            drawing_idx = 0;
        }
    } while (drawing_idx == xfbMng->getDisplayingXfbIndex());
    xfbMng->setDrawingXfbIndex(drawing_idx);
}

void JFWDisplay::copyXfb_triple() {
    JUTXfb *xfbMng = JUTXfb::getManager();

    if (xfbMng->getDrawingXfbIndex() >= 0) {
        if (_44 != NULL) {
            _44();
        }
        prepareCopyDisp();
        GXCopyDisp(xfbMng->getDrawingXfb(), GX_TRUE);
        GXPixModeSync();
    }
}

void JFWDisplay::preGX() {
    GXInvalidateTexAll();
    GXInvalidateVtxCache();

    if (JUTVideo::getManager()->isAntiAliasing())  {
        GXSetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
        GXSetDither(GX_ENABLE);
    }
    else {
        if (mEnableAlpha) {
            GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
            GXSetDither(GX_ENABLE);
        }
        else {
            GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
            GXSetDither(GX_DISABLE);
        }
    }
}

void JFWDisplay::endGX() {
    f32 width = JUTVideo::getManager()->getFbWidth();
    f32 height = JUTVideo::getManager()->getEfbHeight();

    J2DOrthoGraph ortho(0.0f, 0.0f, width, height, -1.0f, 1.0f);

    if (mFader != NULL) {
        ortho.setPort();
        mFader->control();
    }
    ortho.setPort();
    JUTDbPrint::getManager()->flush();

    if (JUTConsoleManager::getManager() != NULL) {
        ortho.setPort();
        JUTConsoleManager::getManager()->draw();
    }

    ortho.setPort();
    JUTProcBar::getManager()->draw();

    if (mDrawDoneMethod != UNK_METHOD_0 || JUTXfb::getManager()->getBufferNum() == 1) {
        JUTAssertion::flushMessage_dbPrint();
    }
    GXFlush();
}

// for MKDD's screenshot function it seems, not sure why it was added here in JSystem
void MyAlloc(u32 p1) {
    JKRHeap::getSystemHeap()->alloc(p1, 0);
}

void MyFree(void * p1) {
    JKRHeap::getSystemHeap()->free(p1);
}

void JFWDisplay::beginRender() {
    JUTProcBar::getManager()->wholeLoopEnd();    
    JUTProcBar::getManager()->wholeLoopStart(0xff, 0x81, 0x1e);
    if (_40) {
        JUTProcBar::getManager()->idleStart(0xff, 0x81, 0x1e);
    }

    waitForTick(_20, m_secondsPer60Frames);
    JUTVideo::getManager()->waitRetraceIfNeed();

    u32 tick = OSGetTick();
    _30 = tick - _2C; // duration of frame in ticks?
    _2C = tick;
    _34 = _2c - JUTVideo::sVideoLastTick; // maybe inline is needed

    if (_40) {
        JUTProcBar::getManager()->idleEnd();
    }

#if DEBUG
        //pVar1 = JUTXfb::getManager()->mBuffer[JUTXfb::sManager->mDrawnXfbIndex];
        SCREENSHOTService(JUTXfb::getManager()->getDrawnXfb(), &MyAlloc, &MyFree);
#endif

    if(_40) {
        JUTProcBar::getManager()->gpStart(0xff, 0x81, 0x1e);
        JUTXfb *xfbMng = JUTXfb::getManager();
        switch (xfbMng->getBufferNum()) {
        case 1:
            if (xfbMng->getSDrawingFlag() != 2) {
                xfbMng->setSDrawingFlag(1);
                clearEfb(mClearColor);
            }
            else {
                xfbMng->setSDrawingFlag(1);
            }
            xfbMng->setDrawingXfbIndex(_48);
            break;
        case 2:
            exchangeXfb_double();
            break;
        case 3:
            exchangeXfb_triple();
            break;
        default:
            break;
        }
    }

    _3C++;
    _40 = _3C >= _38;

    if (_40)  {
        _3C = 0;
    }

    if (_40) {
        clearEfb();
        preGX();
    }
}

void JFWDisplay::endRender() {
    endGX();

    if (_40) {
        switch (JUTXfb::getManager()->getBufferNum())
        {
        case 1:
            drawendXfb_single();
        case 2:
            break;
        case 3:
            copyXfb_triple();
        default:
            break;
        }
    }

    JUTProcBar::getManager()->cpuStart(0xff, 0x81, 0x1e);
    calcCombinationRatio();
}

void JFWDisplay::endFrame() {
    JUTProcBar::getManager()->cpuEnd();

    if (_40) {
        JUTProcBar::getManager()->gpWaitStart(0xff, 0x81, 0x1e);
        switch (JUTXfb::getManager()->getBufferNum()) {
        case 1:
            break;
        case 2:
            JFWDrawDoneAlarm();
            GXFlush();
            break;
        case 3:
            JFWDrawDoneAlarm();
            GXFlush();
            break;
        default:
            break;
        }

        JUTProcBar::getManager()->gpWaitEnd();
        JUTProcBar::getManager()->gpEnd();
    }

    if (_40) {
        static bool init;
        static u32 prevFrame = VIGetRetraceCount();
        u32 retrace_cnt = VIGetRetraceCount();
        JUTProcBar::getManager()->setCostFrame(retrace_cnt - prevFrame);
        prevFrame = retrace_cnt;
    }
}

void waitForTick(u32 p1, u16 p2) {
    if (p1 != 0) {
        static bool init;
        static s64 nextTick = OSGetTime();
        s64 time = OSGetTime();
        while (time < nextTick) {
            JFWDisplay::getManager()->threadSleep((nextTick - time));
            time = OSGetTime();
        }
        nextTick = time + p1;
    }
    else {
        static bool init;
        static u32 nextCount = VIGetRetraceCount();
        u32 uVar1 = (p2 != 0) ? p2 : 1;
        OSMessage msg;
        do {
            if (!OSReceiveMessage(JUTVideo::getManager()->getMessageQueue(), &msg, OS_MESSAGE_BLOCK)) {
                msg = 0;
            }
        } while (((int)msg - (int)nextCount) < 0);
        nextCount = msg + uVar1;
    }
}
