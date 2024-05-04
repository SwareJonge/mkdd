#include "Kameda/Task.h"

Task::Task(int cnt)
{
    mTaskCnt = cnt + 2; // first two indexes are reserved for head and tail
    mCBs = new TCB[mTaskCnt];
    mCBArr = new TCB *[mTaskCnt];
    init();
}

void Task::init()
{
    for (int i = 0; i < mTaskCnt; i++)
    {
        mCBArr[i] = &mCBs[i];
        mCBs[i].mFunc = nullptr;
        mCBs[i].mStackSize = 0x8000;
        mCBs[i].mPrev = nullptr;
        mCBs[i].mNext = nullptr;
        mCBs[i]._c = 0;
        mCBs[i].mRequestKill = false;
        mCBs[i]._10 = 0;
        mCBs[i]._14 = 0;
    }

    mTaskIdx = 0;
    mKillCnt = 0;
    mCBs[0].mFunc = head;
    mCBs[0].mStackSize = 0;
    mCBs[0].mPrev = nullptr;
    mCBs[0].mNext = &mCBs[1];
    mCBs[0]._c = 0;
    mCBs[0].mRequestKill = false;
    mCBs[1].mFunc = tail;
    mCBs[1].mStackSize = 0xffff;
    mCBs[1].mPrev = &mCBs[0];
    mCBs[1].mNext = nullptr;
    mCBs[1]._c = 0;
    mCBs[1].mRequestKill = false;
    mCurCB = &mCBs[0];
    mTaskIdx += 2;
}

Task::~Task() { delete mCurCB; } // UNUSED

Task::TCB *Task::make(TaskFunc func, u16 stackSize)
{
    if (mTaskIdx == mKillCnt)
        return nullptr;

    TCB *newCB = mCBArr[mTaskIdx];
    if (++mTaskIdx >= mTaskCnt)
        mTaskIdx = 0;

    if (stackSize == 0xffff)
        stackSize -= 1u;

    newCB->mFunc = func;
    newCB->mStackSize = stackSize;
    newCB->mRequestKill = false;

    TCB *prev = &mCBs[0];
    TCB *next;
    for (next = prev->mNext; next->mStackSize < stackSize; next = next->mNext)
        prev = prev->mNext;

    newCB->mPrev = prev;
    newCB->mNext = next;
    prev->mNext = newCB;
    next->mPrev = newCB;

    return newCB;
}

void Task::run()
{
    mCurCB = &mCBs[0];
    while (mCurCB->mNext)
    {
        if (!mCurCB->mRequestKill)
            mCurCB->mFunc();
        mCurCB = mCurCB->mNext;
    }

    mCurCB = &mCBs[0];
    while (mCurCB->mNext)
    {
        if (mCurCB->mRequestKill)
            kill(mCurCB);
        mCurCB = mCurCB->mNext;
    }
}

void Task::change(TaskFunc func, TCB *cb)
{
    TCB *old = mCurCB;
    cb->mFunc = func;
    mCurCB = cb;
    mCurCB->mFunc();
    mCurCB = old;
}

void Task::kill(TCB *cb)
{
    cb->mPrev->mNext = cb->mNext;
    cb->mNext->mPrev = cb->mPrev;
    mCBArr[mKillCnt] = cb;

    if (++mKillCnt < mTaskCnt)
        return;

    mKillCnt = 0;
}

void Task::head() {}
void Task::tail() {}