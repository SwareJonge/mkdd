#ifndef JAUDIO_JAISEQDATAMGR_H
#define JAUDIO_JAISEQDATAMGR_H

#include "types.h"

#define JAI_ASYNC_RESULT_0 0
#define JAI_ASYNC_RESULT_RETRY 1
#define JAI_ASYNC_RESULT_OK 2

class JAISoundID;

struct JAISeqData
{
    JAISeqData(void *param_0, u32 param_1)
    {
        _0 = param_0;
        _4 = param_1;
    }

    void set(void *param_0, u32 param_1)
    {
        _0 = param_0;
        _4 = param_1;
    }

    void *_0;
    u32 _4;
};

struct JAISeqDataRegion
{
    bool intersects(JAISeqData &seqData) const
    {
        if ((u32)addr + size < (u32)seqData._0)
        {
            return false;
        }
        if ((u32)seqData._0 + seqData._4 < (u32)addr)
        {
            return false;
        }
        return true;
    }

    u8 *addr;
    u32 size;
};

struct JAISeqDataUser
{
    virtual ~JAISeqDataUser();
    virtual bool isUsingSeqData(const JAISeqDataRegion &) = 0;
    virtual int releaseSeqData(const JAISeqDataRegion &) = 0;
};

struct JAISeqDataMgr
{
    virtual ~JAISeqDataMgr();
    virtual s32 getSeqData(JAISoundID, JAISeqData *) = 0;
    virtual int releaseSeqData() = 0;
    virtual bool setSeqDataUser(JAISeqDataUser *) = 0;
};

#endif