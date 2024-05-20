#ifndef JAUDIO_JASTASKTHREAD_H
#define JAUDIO_JASTASKTHREAD_H

#include "JSystem/JKernel/JKRThread.h"
#include "types.h"

class JASTaskThread : public JKRThread
{
public:
    JASTaskThread(int, int, u32);
    void allocCallStack(void (*)(void *), void *);
    void allocCallStack(void (*)(void *), void const *, u32);
    void sendCmdMsg(void (*)(void *), void *);
    int sendCmdMsg(void (*)(void *), void const *, u32);
    void pause(bool);

    virtual ~JASTaskThread();
    virtual void *run();

    OSThreadQueue mpThreadQueue; // 7C
    bool _84;                    // 84
};

#endif