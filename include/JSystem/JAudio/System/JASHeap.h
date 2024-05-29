#ifndef JAUDIO_JASHEAP_H
#define JAUDIO_JASHEAP_H

#include <JSystem/JKernel/JKRHeap.h>

class JASDisposer;
class JKRSolidHeap;

class JASHeap
{
public:
    JASHeap(JASDisposer *disposer = NULL);
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
    OSMutex mMutex;         // 1C
    JASDisposer *mDisposer; // 34
    u8 *mBase;              // 38
    u32 mSize;              // 3C
    JASHeap *_40;           // 40
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

    //template <typename T>
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
