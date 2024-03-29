#ifndef JAUDIO_JASHEAP_H
#define JAUDIO_JASHEAP_H

#include <JSystem/JKernel/JKRHeap.h>

class JASDisposer;
class JKRSolidHeap;

class JASHeap
{
public:
    JASHeap(JASDisposer *param_0 = NULL);
    void initRootHeap(void *, u32);
    bool alloc(JASHeap *, u32);
    bool allocTail(JASHeap *, u32);
    bool free();
    void insertChild(JASHeap *, JASHeap *, void *, u32, bool);
    JASHeap *getTailHeap();
    u32 getTailOffset();
    u32 getCurOffset();
    ~JASHeap();

    void *getBase() { return mBase; }
    bool isAllocated() { return mBase; }

    JSUTree<JASHeap> mTree; // 00
    OSMutex mMutex; // 1C
    JASDisposer *mDisposer; // 34
    u8 *mBase; // 38
    u32 mSize; // 3C
    JASHeap *_40; // 40
};

namespace JASKernel
{
    void setupRootHeap(JKRSolidHeap *, u32);
    JKRHeap *getSystemHeap();
    void *getCommandHeap();
    void setupAramHeap(u32, u32);
    JASHeap *getAramHeap();

    extern u8 audioAramHeap[68];
    extern u32 sAramBase;
    extern JKRHeap *sSystemHeap;
    extern void *sCommandHeap;
};


namespace JASThreadingModel
{
    template <typename A0>
    struct InterruptsDisable
    {
        struct Lock
        {
            Lock(const A0 &param_0) { _0 = OSDisableInterrupts(); }
            ~Lock() { OSRestoreInterrupts(_0); }

            BOOL _0;
        };
    };

    struct ObjectLevelLockable
    {
        // Should be templated on the chunk memory but couldn't initialize it inside the class itself
        // template <typename A0>
        struct Lock
        {
            Lock(OSMutex *mutex)
            {
                mMutex = mutex;
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
