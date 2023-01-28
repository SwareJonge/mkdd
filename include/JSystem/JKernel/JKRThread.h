#ifndef _JSYSTEM_JKR_JKRTHREAD_H
#define _JSYSTEM_JKR_JKRTHREAD_H

#include "dolphin/os.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

struct JKRThread;
struct JUTConsole;
struct JUTFont;

struct JKRThreadName_
{
};

struct JKRThreadSwitch
{
    typedef void (*Callback)(OSThread *, OSThread *);
    JKRThreadSwitch(JKRHeap *); // unused/inlined
    ~JKRThreadSwitch();         // unused/inlined

    // vtable is optimized out. Was 14 bytes large.
    virtual void v_08(); // _08
    virtual void v_0C(); // _0C
    virtual void v_10(); // _10

    void loopProc();

    // unused/inlined:
    void createManager(JKRHeap *);
    void enter(OSThread *, JKRHeap *, int);
    void enter(JKRThread *, int);
    Callback setPreCallback(Callback);
    Callback setPostCallback(Callback);
    void callback(OSThread *, OSThread *);
    void resetAll();
    void draw(JKRThreadName_ *);
    void draw(JKRThreadName_ *, JUTConsole *);
    void createConsole(JUTFont *, int, JKRHeap *);

    static JKRThreadSwitch *sManager;
};

struct JKRThread : public JKRDisposer
{
    struct TLoad
    {
        inline TLoad()
        {
            mSwitchCount = 0;
            mCost = 0;
            mLastTick = 0;
            mIsValid = false;
            mThreadID = 0;
        }

        bool isValid() const { return mIsValid; }
        u32 getCost() const { return mCost; }
        u32 getCount() const { return mSwitchCount; }
        int getId() const { return mThreadID; }

        void setValid(bool valid) { mIsValid = valid; }
        void setId(int id) { mThreadID = id; }
        void setCurrentTime() { mLastTick = OSGetTick(); }

        void resetCost() { mCost = 0; }
        void resetCount() { mSwitchCount = 0; }

        void incCount() { mSwitchCount++; }
        void addCurrentCost() { mCost = mCost + (OSGetTick() - mLastTick); }

        void clear()
        {
            resetCount();
            resetCost();
            mLastTick = 0;
        }

        bool mIsValid;    // _00
        u32 mCost;        // _04
        u32 mSwitchCount; // _08
        u32 mLastTick;    // _0C
        int mThreadID;    // _10
    };

    JKRThread(u32 stackSize, int msgCount, int threadPriority);
    JKRThread(JKRHeap *, u32 stackSize, int msgCount, int threadPriority);
    JKRThread(OSThread *, int);

    virtual ~JKRThread();                   // _08
    virtual void *run() { return nullptr; } // _0C (weak)

    void setCommon_mesgQueue(JKRHeap *, int);
    BOOL setCommon_heapSpecified(JKRHeap *, u32, int);
    static void *start(void *);

    // unused/inlined:
    static TLoad *searchThread(OSThread *);
    static TLoad *searchThreadLoad(OSThread *);
    void dump();

    OSThread *getThreadRecord() const { return mThread; }
    void *getStack() const { return mStack; }
    TLoad *getLoadInfo() { return &mLoadInfo; }
    JKRHeap *getCurrentHeap() const { return mCurrentHeap; }
    s32 getCurrentHeapError() const { return mCurrentHeapError; }
    void resume() { OSResumeThread(mThread); }

    void setCurrentHeap(JKRHeap *heap)
    {
        if (!heap)
        {
            heap = JKRGetCurrentHeap();
        }

        mCurrentHeap = heap;
    }

    static JSUList<JKRThread> sThreadList;

    // _00     = VTBL
    // _00-_18 = JKRDisposer
    JSULink<JKRThread> mLink; // _18
    JKRHeap *mHeap;           // _28
    OSThread *mThread;        // _2C
    OSMessageQueue mMsgQueue; // _30
    OSMessage *mMsgBuffer;    // _50
    int mMsgCount;            // _54
    void *mStack;             // _58
    u32 mStackSize;           // _5C
    TLoad mLoadInfo;          // _60
    JKRHeap *mCurrentHeap;    // _74
    int mCurrentHeapError;    // _78
};

/**
 * @size{0x98}
 */
struct JKRTask : public JKRThread
{
    typedef void RequestCallback(void *);

    JKRTask(int, int, u32); // unused/inlined

    virtual ~JKRTask();  // _08
    virtual void *run(); // _0C

    void request(RequestCallback *, void *, void *);

    static JKRTask *create(int, int, unsigned long, JKRHeap *);

    // unused/inlined:
    void searchBlank();
    void requestJam(RequestCallback, void *, void *);
    void cancelAll();
    void createTaskEndMessageQueue(int, JKRHeap *);
    void destroyTaskEndMessageQueue();
    void waitQueueMessageBlock(OSMessageQueue *, int *);
    void waitQueueMessage(OSMessageQueue *, int *);

    static void destroy(JKRTask *);

    // u32 _78;			 // _78
    JSULink<JKRTask> _7C; // _7C
    void *_8C;            // _8C - ptr to array with elements of size 0xc
    u32 _90;              // _90 - element count of _8C
    OSMessageQueue *_94;  // _94

    static JSUList<JKRTask> sTaskList;
};

/** @unused */
struct JKRIdleThread : JKRThread
{
    // vtable is optimized out. Was 14 bytes large.
    virtual ~JKRIdleThread(); // _08
    virtual void *run();      // _0C
    virtual void v_10();      // _10

    void destroy() {}

    static void create(JKRHeap *, int, u32);

    static JKRIdleThread *sThread;
};

#endif