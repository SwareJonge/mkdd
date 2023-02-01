#ifndef SYSDEBUG_H
#define SYSDEBUG_H

#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JKernel/JKRHeap.h"

class SysDbUsrPage : JKRDisposer
{
public:
    SysDbUsrPage();
    virtual ~SysDbUsrPage();

private:
    JSULink<SysDbUsrPage> link;
};

class SysDebug
{
public:
    static SysDebug* createManager();
    int appendPage(SysDbUsrPage *);
    void beginUserTime(int);
    void endUserTime(int);
    void createHeapInfo(JKRHeap *, char *);
    void destroyHeapInfo(JKRHeap *);
    void setDefaultHeapGroup(JKRHeap *);
    void setHeapGroup(char const *name, JKRHeap *heap);
    void setUserTimeLabel(int, const char *);
    JKRHeap * getDebugHeap() const {
        return mDebugHeap;
    }

    static SysDebug *getManager()  { return sManager; }
private:
    static SysDebug *sManager;
    u8 _0[0xb8];
    JKRHeap * mDebugHeap;
};

#endif