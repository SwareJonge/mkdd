#include "JSystem/JAudio/System/JASCalc.h"
#include "JSystem/JAudio/System/JASHeap.h"
#include "JSystem/JAudio/System/JASKernel.h"
#include "JSystem/JAudio/System/JASCriticalSection.h"
#include "JSystem/JAudio/System/JASTaskThread.h"

// Everything here matches for TP Debug, the only issue is with the template hell for ObjectLockable(so i don't count it)

JASTaskThread::JASTaskThread(int prio, int msgCount, u32 stackSize) : JKRThread(JASDram, stackSize, msgCount, prio)
{
    mDoSleep = false;
    OSInitThreadQueue(&mpThreadQueue);
}

JASTaskThread::~JASTaskThread()
{
    void *mem;
    BOOL recieved;
    while (TRUE)
    {
        mem = waitMessage(&recieved);
        if (!recieved)
            break;
        JASKernel::getCommandHeap()->free(mem);
    }
}

void *JASTaskThread::allocCallStack(JASTaskCb taskCb, const void *src, u32 size)
{
    u32 stackSize = size + sizeof(JASCallStackBase);
    JASCallStackBase *stack = (JASCallStackBase *)JASKernel::getCommandHeap()->alloc(stackSize);

    if (stack == NULL)
        return NULL;

    stack->_4 = 1;
    JASCalc::bcopy(src, stack + 1, size);
    stack->mCb = taskCb;
    return stack;
}

void *JASTaskThread::allocCallStack(JASTaskCb taskCb, void *p)
{
    JASCallStack *stack = (JASCallStack *)JASKernel::getCommandHeap()->alloc(sizeof(JASCallStack));
    if (stack == NULL)
        return NULL;

    stack->_4 = 0;
    stack->mPtr = p;
    stack->mCb = taskCb;
    return stack;
}

int JASTaskThread::sendCmdMsg(JASTaskCb cb, const void *src, u32 size)
{
    void *stack = allocCallStack(cb, src, size);
    if (stack == NULL)
    {
        return 0;
    }

    BOOL msg = sendMessage(stack);
    if (msg == FALSE)
    {
        JASKernel::getCommandHeap()->free(stack);
    }
    return msg;
}

int JASTaskThread::sendCmdMsg(JASTaskCb cb, void *p)
{
    void *stack = allocCallStack(cb, p);
    if(stack == NULL) {
        return 0;
    }

    BOOL msg = sendMessage(stack);
    if(msg == FALSE) {
        JASKernel::getCommandHeap()->free(stack);
    }
    return msg;
}

void *JASTaskThread::run() {
    void *stack;
    OSInitFastCast();
    while (TRUE) 
    {
        stack = waitMessageBlock();
        if (mDoSleep) {            
            OSSleepThread(&mpThreadQueue);
        }
        if (((JASCallStackBase *)stack)->_4 != 0)
        {
            ((JASCallStackBase *)stack)->mCb((JASCallStackBase *)stack + 1);
        }
        else
        {
            ((JASCallStackBase *)stack)->mCb(((JASCallStack *)stack)->mPtr);
        }
        JASKernel::getCommandHeap()->free(stack);
    }
}

void JASTaskThread::pause(bool doPause)
{
    JAS_CS_START
    if (doPause)
    {
        mDoSleep = true;
    }
    else {
        if(mDoSleep) {
            OSWakeupThread(&mpThreadQueue);
        }
        mDoSleep = false;
    }
    JAS_CS_END
}
