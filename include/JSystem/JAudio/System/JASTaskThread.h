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

    /* 0x7C */ OSThreadQueue mpThreadQueue;
    /* 0x84 */ bool field_0x84;
};

#endif /* JASTASKTHREAD_H */