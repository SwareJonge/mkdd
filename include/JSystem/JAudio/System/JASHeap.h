#ifndef JAUDIO_JASHEAP_H
#define JAUDIO_JASHEAP_H

#include "dolphin/os/OSMutex.h"
#include "dolphin/os/OSThread.h"
#include <JSystem/JKernel/JKRHeap.h>

class JASDisposer;
class JKRSolidHeap;

class JASHeap
{
public:
    JASHeap(JASDisposer *disposer = NULL);
    ~JASHeap() {}
    void initRootHeap(void *, u32);
    bool alloc(JASHeap *, u32);
    bool allocTail(JASHeap *, u32);
    bool free();
    void insertChild(JASHeap *, JASHeap *, void *, u32, bool);
    JASHeap *getTailHeap();
    u32 getTailOffset();
    u32 getCurOffset();

    void *getBase() { return mBase; }
    bool isAllocated() { return mBase; }

    // Inline/Unused
    void dump();
    void dump(int);

    JSUTree<JASHeap> mTree; // 00
    mutable OSMutex mMutex; // 1C
    JASDisposer *mDisposer; // 34
    u8 *mBase;              // 38
    u32 mSize;              // 3C
    JASHeap *_40;           // 40
};

class JASSolidHeap { // Unused class
public:
    JASSolidHeap();
    JASSolidHeap(u8 *, u32);
    void init(u8 *, u32);
    void alloc(u32);
    void freeLast();
    void freeAll();
    u32 getRemain();
};

namespace JASThreadingModel
{
    template <typename T>
    struct InterruptsDisable
    {
        struct Lock
        {
            Lock(const T &param_0) { mInterrupt = OSDisableInterrupts(); }
            ~Lock() { OSRestoreInterrupts(mInterrupt); }

            BOOL mInterrupt;
        };
    };

    template <typename T>
    struct SingleThreaded
    {
        struct Lock
        {
            Lock(const T &) {}
        };
    };

    template <typename A0>
    struct ObjectLevelLockable
    {        
        struct Lock
        {
            Lock(const A0 &obj)
            {
                mMutex = &obj.mMutex;
                OSLockMutex(mMutex);
            }

            ~Lock()
            {
                OSUnlockMutex(mMutex);
            }

            OSMutex *mMutex;
        };
    };
};

extern JKRHeap *JASDram;

#endif
