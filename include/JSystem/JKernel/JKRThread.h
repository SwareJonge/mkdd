#ifndef _JSYSTEM_JKR_JKRTHREAD_H
#define _JSYSTEM_JKR_JKRTHREAD_H

#include "dolphin/os.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "types.h"

struct JKRThread;
struct JUTFont;

struct JKRThreadName_ {
    s32 id;
    char* name;
};

typedef void (*JKRThreadSwitch_PreCallback)(OSThread* current, OSThread* next);
typedef void (*JKRThreadSwitch_PostCallback)(OSThread* current, OSThread* next);

class JKRThreadSwitch {
public:
    JKRThreadSwitch(JKRHeap*);
    virtual void draw(JKRThreadName_* param_1, JUTConsole* param_2);
    virtual void draw(JKRThreadName_* param_1);
    virtual ~JKRThreadSwitch();

    static JKRThreadSwitch* createManager(JKRHeap* heap);

    JKRThread* enter(JKRThread* param_1, int param_2);
    static void callback(OSThread* param_1, OSThread* param_2);

    static u32 getTotalCount() { return sTotalCount; }

private:
    static JKRThreadSwitch* sManager;
    static u32 sTotalCount;
    static u32 sTotalStart;
    static JKRThreadSwitch_PreCallback mUserPreCallback;
    static JKRThreadSwitch_PostCallback mUserPostCallback;

private:
    /* 0x00 */  // vtable
    /* 0x04 */ JKRHeap* mHeap;
    /* 0x08 */ bool mSetNextHeap;
    /* 0x09 */ u8 field_0x9[3];
    /* 0x0C */ u32 field_0xC;
    /* 0x10 */ u32 field_0x10;
    /* 0x14 */ u8 field_0x14[4];
    /* 0x18 */ s64 field_0x18;
    /* 0x20 */ u32 field_0x20;
    /* 0x24 */ u32 field_0x24;
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
    static JSUList<JKRThread>& getList() { return (JSUList<JKRThread>&)sThreadList; }

    // unused/inlined:
    static JKRThread *searchThread(OSThread *);
    static TLoad *searchThreadLoad(OSThread *);
    void dump();

    OSThread *getThreadRecord() const { return mThreadRecord; }
    void *getStack() const { return mStackMemory; }
    TLoad *getLoadInfo() { return &mLoadInfo; }
    JKRHeap *getCurrentHeap() const { return mCurrentHeap; }
    s32 getCurrentHeapError() const { return mCurrentHeapError; }
    void resume() { OSResumeThread(mThreadRecord); }

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
    JSULink<JKRThread> mThreadListLink; // _18
    JKRHeap *mHeap;           // _28
    OSThread *mThreadRecord;        // _2C
    OSMessageQueue mMessageQueue; // _30
    OSMessage *mMesgBuffer;    // _50
    s32 mMessageCount;            // _54
    void *mStackMemory;             // _58
    u32 mStackSize;           // _5C
    TLoad mLoadInfo;          // _60
    JKRHeap *mCurrentHeap;    // _74
    s32  mCurrentHeapError;    // _78
};

/**
 * @size{0x98}
 */
struct JKRTask : public JKRThread
{
    typedef void (*RequestCallback)(void*);

	/**
	 * @fabricated
	 * @size{0xC}
	 */
    struct Message {
		RequestCallback _00;
		void* _04;
		void* _08;
	};


    JKRTask(int, int, u32); // unused/inlined

    virtual ~JKRTask();  // _08
    virtual void *run(); // _0C

    bool request(RequestCallback, void *, void *);

    static JKRTask *create(int, int, unsigned long, JKRHeap *);

    // unused/inlined:
    Message* searchBlank();
    void requestJam(RequestCallback, void *, void *);
    void cancelAll();
    void createTaskEndMessageQueue(int, JKRHeap *);
    void destroyTaskEndMessageQueue();
    void waitQueueMessageBlock(OSMessageQueue *, int *);
    void waitQueueMessage(OSMessageQueue *, int *);

    OSMessage waitMessageBlock() {
        OSMessage msg;
        OSReceiveMessage(&mMessageQueue, &msg, OS_MESSAGE_BLOCK);
        return msg;
    }

    static void destroy(JKRTask *);

    // u32 _78;			 // _78
    JSULink<JKRTask> _7C; // _7C
    Message *_8C;            // _8C - ptr to array with elements of size 0xc
    u32 _90;              // _90 - element count of _8C
    OSMessageQueue *_94;  // _94

    static JSUList<JKRTask> sTaskList;
    static u8 sEndMesgQueue[32]; // Unused
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