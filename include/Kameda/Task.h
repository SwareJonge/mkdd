#ifndef TASK_H
#define TASK_H

#include "types.h"

class Task {
public:
    typedef void (*TaskFunc)();

    struct TCB
    {        
        TaskFunc mFunc;
        TCB *mPrev;
        TCB *mNext;
        u32 _c;
        u32 _10;
        u32 _14;
        u16 mStackSize;
        bool mRequestKill;
        int mExecIdx; // The amount of times a function has to be executed before it gets killed?
        int _20;
        s16 _24; // pre
        s16 _26; // main
        s16 _28; // after
        u8 _2a[0x3c - 0x2a]; // unknown
    }; // Size: 0x3c

    Task(int cnt);
    ~Task();
    void init();
    TCB *make(TaskFunc func, u16 stackSize);
    void run();
    void change(TaskFunc func, TCB *cb);
    void kill(TCB *cb);

    static void head();
    static void tail();

    int mTaskCnt;
    TCB *mCBs;
    TCB **mCBArr;
    int mTaskIdx;
    int mKillCnt;
    TCB *mCurCB;
};

#endif