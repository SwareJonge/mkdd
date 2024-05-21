#ifndef JAUDIO_JAISTREAMDATAMGR_H
#define JAUDIO_JAISTREAMDATAMGR_H

#include "JSystem/JAudio/Interface/JAISound.h"

struct JAIStreamDataMgr
{
    virtual s32 getStreamFileEntry(JAISoundID soundID) = 0;
    virtual ~JAIStreamDataMgr();
};

struct JAIStreamAramMgr
{
    virtual u32 newStreamAram(u32 *pSize) = 0;
    virtual int deleteStreamAram(u32 addr) = 0;
    virtual ~JAIStreamAramMgr();
};

#endif