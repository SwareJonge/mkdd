#ifndef JAUDIO_JAISEQDATAMGR_H
#define JAUDIO_JAISEQDATAMGR_H

#include "JSystem/JAudio/Interface/JAISound.h"

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
        if (_0 + _4 < (u32)seqData._0)
        {
            return false;
        }
        if ((u32)seqData._0 + seqData._4 < _0)
        {
            return false;
        }
        return true;
    }

    u32 _0;
    u32 _4;
};

struct JAISeqDataUser
{
    virtual ~JAISeqDataUser();
    virtual bool isUsingSeqData(JAISeqDataRegion const &) = 0;
    virtual int releaseSeqData(JAISeqDataRegion const &) = 0;
};

struct JAISeqDataMgr
{
    enum SeqDataReturnValue
    {
        SeqDataReturnValue_0 = 0,
        SeqDataReturnValue_1 = 1,
        SeqDataReturnValue_2 = 2
    };
    virtual ~JAISeqDataMgr();
    virtual SeqDataReturnValue getSeqData(JAISoundID, JAISeqData *) = 0;
    virtual int releaseSeqData() = 0;
    virtual bool setSeqDataUser(JAISeqDataUser *) = 0;
};

#endif /* JAUDIO_JAISEQDATAMGR_H */