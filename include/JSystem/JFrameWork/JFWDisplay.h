#ifndef _JSYSTEM_JFW_JFWDISPLAY_H
#define _JSYSTEM_JFW_JFWDISPLAY_H

#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTDirectPrint.h"
#include "JSystem/JUtility/JUTFader.h"
#include "JSystem/JUtility/JUTProcBar.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JUtility/JUTXfb.h"
#include "JSystem/JUtility/TColor.h"
#include "dolphin/GX.h"
#include "types.h"

typedef void (*JFWDisplayUnkFunc)(void);

class JFWAlarm : public OSAlarm
{
public:
    JFWAlarm() : mLink(this) {}
    ~JFWAlarm() {}
    void createAlarm() { OSCreateAlarm(this); }
    void cancelAlarm() { OSCancelAlarm(this); }
    void removeLink() { sList.remove(&mLink); }
    void appendLink() { sList.append(&mLink); }
    OSThread *getThread() const { return mThread; }
    void setThread(OSThread *thread) { mThread = thread; }

    static JSUList<JFWAlarm> sList;

public:
    /* 0x28 */ OSThread *mThread;
    /* 0x2C */ JSULink<JFWAlarm> mLink;
};

class JFWDisplay
{
public:
    enum EDrawDone
    {
        /* 0x0 */ UNK_METHOD_0 = 0,
        /* 0x1 */ UNK_METHOD_1 = 1
    };

    /* 80272040 */ void ctor_subroutine(bool);
    /* 802720F8 */ JFWDisplay(JKRHeap *, JUTXfb::EXfbNumber, bool);
    /* 802721DC */ static JFWDisplay *createManager(_GXRenderModeObj const *, JKRHeap *,
                                                    JUTXfb::EXfbNumber, bool);
    /* 802722B8 */ void prepareCopyDisp();
    /* 802723AC */ void drawendXfb_single();
    /* 802723F4 */ void exchangeXfb_double();
    /* 802724FC */ void exchangeXfb_triple();
    /* 80272574 */ void copyXfb_triple();
    /* 802725F8 */ void preGX();
    /* 8027268C */ void endGX();
    /* 80272C60 */ void waitBlanking(int);
    /* 80272E10 */ void threadSleep(s64);
    /* 80272EB8 */ void clearEfb_init();
    /* 80272F9C */ void clearEfb(int, int, int, int, _GXColor);
    /* 80272F2C */ void clearEfb();
    /* 80272F58 */ void clearEfb(_GXColor);
    /* 8027331C */ void calcCombinationRatio();

    /* 80272798 */ virtual void beginRender();
    /* 80272A04 */ virtual void endRender();
    /* 80272AB0 */ virtual void endFrame();
    /* 80272160 */ virtual ~JFWDisplay();

    static JFWDisplay *getManager() { return sManager; }

    int getEfbHeight() const {
        return JUTVideo::getManager()->getEfbHeight();
    }

    int getEfbWidth() const {
        return JUTVideo::getManager()->getFbWidth();
    }

    int startFadeOut(int param_0) {
        if (mFader != nullptr) {
            return mFader->startFadeOut(param_0);
        }
        return 1;
    }

    int startFadeIn(int param_0) {
        if (mFader != nullptr) {
            return mFader->startFadeIn(param_0);
        }
        return 1;
    }

    void setTickRate(u32 rate) {
        mTickRate = rate;
        mFrameRate = 0;
    }

    static JFWDisplay *sManager;

private:
    /* 0x04 */ JUTFader *mFader;
    /* 0x08 */ JUtility::TColor mClearColor;
    /* 0x0C */ u32 mZClear;
    /* 0x10 */ JUTXfb *mXfb;
    /* 0x14 */ u16 mGamma;
    /* 0x18 */ EDrawDone mDrawDoneMethod;
    /* 0x1C */ u16 mFrameRate;
    /* 0x20 */ u32 mTickRate;
    /* 0x24 */ bool mEnableAlpha;
    /* 0x26 */ u16 mClamp;
    /* 0x28 */ f32 mCombinationRatio;
    /* 0x2C */ u32 _2C;
    /* 0x30 */ u32 _30;
    /* 0x34 */ u32 _34;
    /* 0x38 */ int _38;
    /* 0x3C */ int _3C;
    /* 0x40 */ bool _40;
    /* 0x44 */ JFWDisplayUnkFunc _44;
    /* 0x48 */ s16 _48;
    /* 0x4A */ u8 _4a;
};

static void JFWDrawDoneAlarm();
static void JFWThreadAlarmHandler(OSAlarm *, OSContext *);
static void JFWGXAbortAlarmHandler(OSAlarm *, OSContext *);
static void waitForTick(u32, u16);
static void diagnoseGpHang();

#endif