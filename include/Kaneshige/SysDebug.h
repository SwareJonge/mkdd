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
    void ctrlDebugMode();
    void endUserTime(int);
    void clrAllUserTimeLabel();
    void createHeapInfo(JKRHeap *, char *);
    void destroyHeapInfo(JKRHeap *);
    void draw();
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

#define SYSDBG_SetHeapGroup(name, heap) \
    SysDebug::getManager()->setHeapGroup(name, heap);

#define SYSDBG_SetDefaultHeapGroup(heap) \
    SysDebug::getManager()->setDefaultHeapGroup(heap);

#ifdef DEBUG
#define SYSDBG_CreateHeapInfo(heap, name) \
    SysDebug::getManager()->createHeapInfo(heap, name);
#define SYSDBG_SetUserTimeLabel(id, name) \
    SysDebug::getManager()->setUserTimeLabel(id, name);
#define SYSDBG_BeginUserTime(id) \
    SysDebug::getManager()->beginUserTime(id);
#define SYSDBG_EndUserTime(id) \
    SysDebug::getManager()->endUserTime(id);

#else
#define SYSDBG_CreateHeapInfo(...)
#define SYSDBG_SetUserTimeLabel(...)
#define SYSDBG_BeginUserTime(...)
#define SYSDBG_EndUserTime(...)
#endif

#endif