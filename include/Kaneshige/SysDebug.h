#ifndef SYSDEBUG_H
#define SYSDEBUG_H

#include "JSystem/JKernel/JKRDisposer.h"

class SysDbUsrPage : JKRDisposer
{
public:
    SysDbUsrPage();
    ~SysDbUsrPage();

private:
    JSULink<SysDbUsrPage> link;
};

class SysDebug
{
public:
    static SysDebug *getManager(); /* {
        return sManager;
    }*/
    int appendPage(SysDbUsrPage *);

private:
    static SysDebug *sManager;
};

#endif