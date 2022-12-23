#ifndef SYSDEBUG_H
#define SYSDEBUG_H

#include "JSystem/JKernel/JKRDisposer.h"

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
    static SysDebug *getManager() {
        return sManager;
    }
    int appendPage(SysDbUsrPage *);
    void createHeapInfo(JKRHeap *, char *);
    void destroyHeapInfo(JKRHeap *);

private:
    static SysDebug *sManager;
};

#endif