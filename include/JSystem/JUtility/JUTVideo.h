#ifndef _JUTVIDEO_H
#define _JUTVIDEO_H

#include "dolphin/os.h"
#include "dolphin/gx.h"
#include "dolphin/vi.h"
#include "types.h"

/**
 * @size{0x58}
 */

typedef u8 (*Pattern)[2];

struct JUTVideo
{
    JUTVideo(const _GXRenderModeObj *);

    virtual ~JUTVideo(); // _08

    static JUTVideo *createManager(const _GXRenderModeObj *);
    static void destroyManager();
    static void preRetraceProc(unsigned long);
    static void postRetraceProc(unsigned long);
    static void drawDoneCallback();

    u16 getEfbHeight() const { return m_renderModeObj->efbHeight; }
    u16 getFbWidth() const { return m_renderModeObj->fbWidth; }
    void getBounds(u16& width, u16& height) const {
        width = getFbWidth();
        height = getEfbHeight();
    }
    _GXRenderModeObj *getRenderMode() const { return m_renderModeObj; }
    u16 getXfbHeight() const { return m_renderModeObj->xfbHeight; }
    u32 isAntiAliasing() const { return m_renderModeObj->aa; }
    Pattern getSamplePattern() const { return m_renderModeObj->sample_pattern; }
    u8 *getVFilter() const { return m_renderModeObj->vfilter; }
    OSMessageQueue *getMessageQueue() { return &m_messageQueue; };
    static void drawDoneStart();
    static void dummyNoDrawWait();
    void setRenderMode(const _GXRenderModeObj *);
    void waitRetraceIfNeed(); // blr, global
    VIRetraceCallback setPostRetraceCallback(VIRetraceCallback);

    // Unused/inlined:
    void getDrawWait();
    VIRetraceCallback setPreRetraceCallback(VIRetraceCallback);
    void getPixelAspect(const _GXRenderModeObj *);
    void getPixelAspect() const;

    // Static inline gets
    static JUTVideo *getManager() { return sManager; }
    static OSTick getVideoInterval() { return sVideoInterval; }
    static OSTick getVideoLastTick() { return sVideoLastTick; }

    // _00 VTBL
    _GXRenderModeObj *m_renderModeObj;         // _04
    u32 _08;                                         // _08
    u32 m_retraceCount;                              // _0C
    int _10;                                         // _10
    u8 _14[4];                                       // _14
    u32 _18;                                         // _18
    VIRetraceCallback m_previousPreRetraceCallback;  // _1C
    VIRetraceCallback m_previousPostRetraceCallback; // _20
    VIRetraceCallback m_preRetraceCallback;          // _24
    VIRetraceCallback m_postRetraceCallback;         // _28
    bool _2C;                                        // _2C
    s32 _30;                                         // _30
    void *m_messageSlots;                            // _34
    OSMessageQueue m_messageQueue;                   // _38

    static JUTVideo *sManager;
    static s32 sVideoLastTick;
    static u32 sVideoInterval;
};

#endif