#ifndef JAUDIO_JASTASKTHREAD_H
#define JAUDIO_JASTASKTHREAD_H

#include "JSystem/JKernel/JKRThread.h"
#include "types.h"

typedef void (*JASTaskCb)(void *);

// TODO: better names(divide)

struct JASCallStackBase
{
    JASTaskCb mCb; // 0
    int _4;        // 4
}; // Size: 08

struct JASCallStack : JASCallStackBase
{
    void *mPtr;
}; // Size: 0C

class JASTaskThread : public JKRThread
{
public:
    JASTaskThread(int, int, u32);
    void *allocCallStack(JASTaskCb cb, void *mem);
    void *allocCallStack(JASTaskCb cb, const void *mem, u32 size);
    int sendCmdMsg(JASTaskCb cb, void *);
    int sendCmdMsg(JASTaskCb cb, const void *, u32);
    void pause(bool);

    virtual ~JASTaskThread();
    virtual void *run();

    OSThreadQueue mpThreadQueue; // 7C
    bool mDoSleep;               // 84
};

#endif