#ifndef _JSYSTEM_JUT_JUTXFB_H
#define _JSYSTEM_JUT_JUTXFB_H

#include <dolphin/gx.h>
#include <JSystem/JKernel/JKRHeap.h>
#include "types.h"

class JUTXfb
{
public:
    enum EXfbNumber
    {
        Unset = 0,
        SingleBuffer = 1,
        DoubleBuffer = 2,
        TripleBuffer = 3
    };

    /* 802E5214 */ void clearIndex();
    /* 802E5228 */ void common_init(int);
    /* 802E5260 */ JUTXfb(_GXRenderModeObj const *, JKRHeap *, JUTXfb::EXfbNumber);
    /* 802E5308 */ ~JUTXfb();
    /* 802E5378 */ void delXfb(int);
    /* 802E53B8 */ static JUTXfb *createManager(JKRHeap *, JUTXfb::EXfbNumber);
    /* 802E5424 */ static void destroyManager();
    /* 802E5454 */ void initiate(u16, u16, JKRHeap *, JUTXfb::EXfbNumber);

    s32 getBufferNum() const { return mBufferNum; }
    int getDrawnXfbIndex() const { return mDrawnXfbIndex; }
    int getDrawingXfbIndex() const { return mDrawingXfbIndex; }
    int getDisplayingXfbIndex() const { return mDisplayingXfbIndex; }
    s32 getSDrawingFlag() const { return mSDrawingFlag; }

    void *getDrawnXfb() const {
        return (mDrawnXfbIndex >= 0) ? mBuffer[mDrawnXfbIndex] : nullptr;
    }

    void *getDrawingXfb() const {
        return (mDrawingXfbIndex >= 0) ? mBuffer[mDrawingXfbIndex] : nullptr;
    }

    void *getDisplayingXfb() const {
        return (mDisplayingXfbIndex >= 0) ? mBuffer[mDisplayingXfbIndex] : nullptr;
    }

    void setDisplayingXfbIndex(s16 index) { mDisplayingXfbIndex = index; }
    void setSDrawingFlag(s32 flag) { mSDrawingFlag = flag; }
    void setDrawnXfbIndex(s16 index) { mDrawnXfbIndex = index; }
    void setDrawingXfbIndex(s16 index) { mDrawingXfbIndex = index; }

    static JUTXfb *getManager() { return sManager; }

private:
    static JUTXfb *sManager;

private:
    /* 0x00 */ void *mBuffer[3];
    /* 0x0C */ bool mXfbAllocated[3];
    /* 0x10 */ s32 mBufferNum;
    /* 0x14 */ s16 mDrawingXfbIndex;
    /* 0x16 */ s16 mDrawnXfbIndex;
    /* 0x18 */ s16 mDisplayingXfbIndex;
    /* 0x1C */ s32 mSDrawingFlag;
};

#endif
